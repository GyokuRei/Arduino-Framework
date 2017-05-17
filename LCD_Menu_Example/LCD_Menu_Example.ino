// khai bao thu vien 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "LCDMenu.h"
#include "StableButton.h"

// khai bao chan su dung
#define RETURN 4
#define LEFT 5
#define RIGHT 6
#define DOWN 7
#define UP 8
#define ENTER 9

uint8_t ButtonArray[] = { RETURN, LEFT, RIGHT, UP, DOWN, ENTER };

LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(0x3F, 16, 2);

Label* lbPinAnalogValue;

void setup()
{
	Serial.begin(9600);

	/*
	secondMenu
	┌----------------┐
	│Do you love me? │
	│  Yes       No  │
	└----------------┘
	smYes                          smNo
	┌----------------┐				┌----------------┐
	│    Come on !   │				│ My heart   80  │
	│                │				│ Your heart 30  │
	└----------------┘				└----------------┘
	*/

	OriginMenu* firstMenu = new OriginMenu();
	{
		Label* lbValue = new Label(firstMenu, "Value:", 0, 0);
		lbPinAnalogValue = new Label(firstMenu, "0", 7, 0);

		SubMenu* sbPin = new SubMenu(firstMenu, "Pin", 12, 0);
		{
			SubMenu* smPinA1 = new SubMenu(sbPin->Container, "A0", 0, 0);
			{
				Label* lbInfoA0 = new Label(smPinA1->Container, "Analog pin !", 0, 0);
				SubMenu* smPinOrder = new SubMenu(smPinA1->Container, "PinOrder", 0, 1);
				SubMenu* smPort = new SubMenu(smPinA1->Container, "Port", 9, 1);
			}
		}

		FunctionText* fReverseLed = new FunctionText(firstMenu, "Reverse", 0, 1);
		fReverseLed->Function = ReverseLed;

		VariableText* vIntensity = new VariableText(firstMenu, 80, 11, 1);
	}

	OriginMenu* secondMenu = new OriginMenu();
	{
		Label* lbQuestion = new Label(secondMenu, "Do you love me?", 0, 0);
		SubMenu* smYes = new SubMenu(secondMenu, "Yes", 3, 1);
		{
			FunctionText* fComeOn = new FunctionText(smYes->Container, "Come one !", 3, 0);
			fComeOn->Function = ReverseLed;
		}
		SubMenu* smNo = new SubMenu(secondMenu, "No", 9, 1);
		{
			Label* lbMyHeart = new Label(smNo->Container, "My heart:", 0, 0);
			VariableText* vMyHeartValue = new VariableText(smNo->Container, 80, 12, 0);

			Label* lbYourHeart = new Label(smNo->Container, "Your heart:", 0, 1);
			VariableText* vYourHeartValue = new VariableText(smNo->Container, 30, 12, 1);
		}
	}

	OriginMenu* thirdMenu = new OriginMenu();
	{
		Label* lbTemp = new Label(thirdMenu, "Temp :", 0, 0);
		Label* lbValueTemp = new Label(thirdMenu, "22", 7, 0);
		Label* lbTempUnit = new Label(thirdMenu, "deg C", 10, 0);

		Label* lbHumi = new Label(thirdMenu, "Humi :", 0, 1);
		Label* lbValueHumi = new Label(thirdMenu, "60", 7, 1);
		Label* lbHumiUnit = new Label(thirdMenu, "%", 10, 1);
	}

	LCDMenu.Init(lcd, "LCD Menu Example");
	LCDMenu.AddMenu(firstMenu);
	LCDMenu.AddMenu(secondMenu);
	LCDMenu.AddMenu(thirdMenu);
	LCDMenu.SetCurrentMenu(firstMenu);
	LCDMenu.UpdateScreen();

	StableButton.Init(ButtonArray, 6);

	InitIO();

}

void loop()
{
	ExecuteMenuButton();
	LCDMenu.ExecuteEffect();
	UpdateLabel();
}

void InitIO()
{
	pinMode(13, OUTPUT);
}

void ReverseLed()
{
	digitalWrite(13, digitalRead(13) ^ 1);
}

void UpdateLabel()
{
	static unsigned long lasttime = millis();
	if (millis() - lasttime > 200)
	{
		lasttime = millis();
		lbPinAnalogValue->Text = String(analogRead(A0));
		lbPinAnalogValue->IsTextChanged = true;
		LCDMenu.UpdateScreen();
	}
}

void ExecuteMenuButton()
{
	if (StableButton.IsPressed(LEFT))
	{
		LCDMenu.MoveCursorLeft();
	}

	if (StableButton.IsPressed(RIGHT))
	{
		LCDMenu.MoveCursorRight();
	}

	if (StableButton.IsPressed(UP))
	{
		LCDMenu.MoveCursorUp();
	}

	if (StableButton.IsPressed(DOWN))
	{
		LCDMenu.MoveCursorDown();
	}

	if (StableButton.IsPressed(RETURN))
	{
		LCDMenu.Return();
	}

	if (StableButton.IsPressed(ENTER))
	{
		LCDMenu.Enter();
	}
}
