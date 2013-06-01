
//brewery control program 1.4
//update 1/6/13
//
//slave[1] input1 = hot liquor tank NTC
//slave[1] input2 = strike/sparge water NTC
//slave[2] input1 = cold water supply flow meter
//slave[2] input2 = 100lt mash tun NTC
//slave[3] input1 = 400lt mash tun NTC
//
//slave[1] i2c = hot liquor tank pressure sensor
//
//slave[1] output1 = hot liquor tank supply solenoid
//slave[1] output2 = hot liquor tank heat solenoid
//slave[2] output1 = hot liquor tank stirrer
//slave[2] output2 = hot liquor tank drain solenoid
//slave[3] output1 = cold supply solenoid
//slave[3] output2 = 100lt mash tun heat solenoid
//slave[4] output1 = 400lt mash tun heat solenoid
//slave[4] output2 = burner

#include "slave.h"

// declare slaves
const int NUMBEROFSLAVES = 6;
Slave slave[NUMBEROFSLAVES + 1];

// declare global variables
//slave[1] hot liquor tank temperature
int hotLiquorTankTemp;
int hotLiquorTankSetPoint = 800;
int hotLiquorTankHeatCntrl = false;
// slave[2] hot liquor tank volume
int hotLiquorTankVol;
int hotLiquorTankCal;
int hotLiquorTankFillPoint = 100;
int hotLiquorTankFillCntrl = false;
// slave[3] strike/sparge water temperature
int strikeSpargeTemp;
int strikeSpargeSetPoint = 500;
// slave[4] strike/sparge water volume
int strikeSpargeVol;
int strikeSpargeVolRequired = 10;
int hotLiquorTankDrain = false;
int strikeSpargeCntrl = false;
int strikeSpargeDump = false;
// slave[5] 100lt mash tun temperature
int lt100MashTunTemp;
int lt100MashTunSetPoint = 500;
int lt100MashTunHeatCntrl = false;
// slave[6] 400lt mash tun temperature
int lt400MashTunTemp;
int lt400MashTunSetPoint = 500;
int lt400MashTunHeatCntrl = false;

// declare menus
MenuItem menupwm_One[] = {
  {"Temp", Deci, ReadOnly, &hotLiquorTankTemp},
  {"Set", Deci, ReadWrite, &hotLiquorTankSetPoint},
  {"Heat", Bool, ReadWrite, &hotLiquorTankHeatCntrl},
  {0} // must end with a zero to indicate end of list
};

MenuItem menuTwo[] = {
  {"Vol", Unit, ReadOnly, &hotLiquorTankVol},
  {"Set", Unit, ReadWrite, &hotLiquorTankFillPoint},
  {"Fill", Bool, ReadWrite, &hotLiquorTankFillCntrl},
  {"Cal", Unit, ReadOnly, &hotLiquorTankCal},
  {0} // must end with a zero to indicate end of list
};

MenuItem menuThree[] = {
  {"Temp", Deci, ReadOnly, &strikeSpargeTemp},
  {"Set", Deci, ReadWrite, &strikeSpargeSetPoint},
  {0} // must end with a zero to indicate end of list
};

MenuItem menuFour[] = {
  {"Vol", Unit, ReadOnly, &strikeSpargeVol},
  {"Set", Unit, ReadWrite, &strikeSpargeVolRequired},
  {"Tran", Bool, ReadWrite, &strikeSpargeCntrl},
  {"Dump", Bool, ReadWrite, &strikeSpargeDump},
  {0} // must end with a zero to indicate end of list
};

MenuItem menuFive[] = {
  {"Temp", Deci, ReadOnly, &lt100MashTunTemp},
  {"Set", Deci, ReadWrite, &lt100MashTunSetPoint},
  {"Heat", Bool, ReadWrite, &lt100MashTunHeatCntrl},
  {0} // must end with a zero to indicate end of list
};

MenuItem menuSix[] = {
//  {"Temp", Deci, ReadOnly, &lt400MashTunTemp},
//  {"Set", Deci, ReadWrite, &lt400MashTunSetPoint},
//  {"Heat", Bool, ReadWrite, &lt400MashTunHeatCntrl},
  {0} // must end with a zero to indicate end of list
};

MenuItem* menus[] = {0, menupwm_One, menuTwo, menuThree, menuFour, menuFive, menuSix};

void setup() {
  // initialise slaves and menus
  delay(2000);
  
  for(int i = 1 ; i <= NUMBEROFSLAVES ; i++){
    slave[i].begin(i);
    slave[i].addMenu(menus[i]);
  }
}

void loop() {
  
  // update measurements
  hotLiquorTankTemp = slave[1].get(Philips1);
  strikeSpargeTemp = slave[1].get(Honeywell2);
  lt100MashTunTemp = slave[2].get(Philips2);
  lt400MashTunTemp = slave[3].get(Philips1);
  hotLiquorTankCal = slave[1].get(InputI2C);
  int strikeSpargeRawFlow = slave[2].get(Counter1);
  
 
  // update logic
  // hot liquor tank volume and fill control
  int hotLiquorTankMapVol = map(hotLiquorTankCal,330,802,0,500);
  hotLiquorTankVol = constrain(hotLiquorTankMapVol,0,600);
  if (hotLiquorTankFillCntrl == true && hotLiquorTankVol < hotLiquorTankFillPoint){
    slave[1].set(Output1, pwm_On);
  }
  else
  {
    slave[1].set(Output1, pwm_Off);
    hotLiquorTankFillCntrl = false;
  }
  // hot liquor tank thermostat
  const int hotLiquorTankDiff = 20;
  static boolean hotLiquorTankHeat = false;
  if (hotLiquorTankHeatCntrl == true && hotLiquorTankVol > 0 && hotLiquorTankTemp < hotLiquorTankSetPoint){
    slave[1].set(Output2, pwm_On);
    hotLiquorTankHeat = true;
  }
  if (hotLiquorTankHeatCntrl == false || hotLiquorTankVol < 0 || hotLiquorTankTemp > hotLiquorTankSetPoint + hotLiquorTankDiff){
    slave[1].set(Output2, pwm_Off);
    hotLiquorTankHeat = false;
  }
  // strike & sparge water thermostat and dump volume
  static unsigned long strikeSpargeRawFlowTotal;
  static int strikeSpargeFlowTotal;
  static int hotLiquorTankOriginalVol;
  static int hotLiquorTankVolChange;
  switch(strikeSpargeCntrl){
    case true:
    strikeSpargeRawFlowTotal = strikeSpargeRawFlowTotal + strikeSpargeRawFlow;
    strikeSpargeFlowTotal = map(strikeSpargeRawFlowTotal,0,4120,0,10);
    hotLiquorTankVolChange = hotLiquorTankOriginalVol - hotLiquorTankVol;
    strikeSpargeVol = strikeSpargeFlowTotal + hotLiquorTankVolChange; 
    if (strikeSpargeVolRequired > strikeSpargeVol){
      slave[2].set(Output2, pwm_On);
      if (strikeSpargeTemp > strikeSpargeSetPoint){
        slave[3].set(Output1, pwm_On);
      }
      else 
      {
        slave[3].set(Output1, pwm_Off);
      }
    } 
    else
    {
      slave[2].set(Output2, pwm_Off);
      slave[3].set(Output1, pwm_Off);
      strikeSpargeCntrl = false;
    }
    break;
    
    case false:
    strikeSpargeRawFlowTotal = 0;
    hotLiquorTankOriginalVol = hotLiquorTankVol;
    if (strikeSpargeDump == 1){
      slave[2].set(Output2, pwm_On);
    }
      else
    {
      slave[2].set(Output2, pwm_Off);
    }
    break;
  }

  // 100 lt mash tun thermostat
  const int lt100MashTunDiff = 20;
  static boolean lt100MashTunHeat = false;
  if (lt100MashTunHeatCntrl == true && lt100MashTunTemp < lt100MashTunSetPoint){
    slave[3].set(Output2, pwm_On);
    lt100MashTunHeat = true;
  }
  if (lt100MashTunHeatCntrl == false || lt100MashTunTemp > lt100MashTunSetPoint + lt100MashTunDiff){
    slave[3].set(Output2, pwm_Off);
    lt100MashTunHeat = false;
  }
  // 400 lt mash tun thermostat
  const int lt400MashTunDiff = 20;
  static boolean lt400MashTunHeat = false;
  if (lt400MashTunHeatCntrl == true && lt400MashTunTemp < lt400MashTunSetPoint){
    slave[4].set(Output1, pwm_On);
    lt400MashTunHeat = true;
  }
  if (lt400MashTunHeatCntrl == false || lt400MashTunTemp > lt400MashTunSetPoint + lt400MashTunDiff){
    slave[4].set(Output1, pwm_Off);
    lt400MashTunHeat = false;
  }
  
  //  call burner on/off
  if (hotLiquorTankHeat == true || lt100MashTunHeat == true || lt400MashTunHeat == true){
    slave[4].set(Output2, pwm_On); 
  }
  else
  {
    slave[4].set(Output2, pwm_Off);
  }

  
  // update menus
  for(int i = 1 ; i <= NUMBEROFSLAVES ; i++){
    slave[i].update();
  }
  
  delay(100);
}
