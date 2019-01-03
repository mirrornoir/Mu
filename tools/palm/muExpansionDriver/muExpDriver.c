#include <PalmOS.h>
#include <PalmCompatibility.h>
#include <PceNativeCall.h>
#include <stdint.h>

/*dont include this anywhere else*/
#include "MuExpDriverRsc.h"

#include "traps.h"
#include "palmGlobalDefines.h"
#include "specs/emuFeatureRegisterSpec.h"

/*config vars*/
#define CONFIG_FILE_SIZE (20 * sizeof(uint32_t))

enum{
   ARM_STACK_SIZE = 0,
   LCD_WIDTH,
   LCD_HEIGHT
};


static uint32_t* configFile;


static void installTrapHandlers(){
   uint32_t enabledFeatures = readArbitraryMemory32(EMU_REG_ADDR(EMU_INFO));
   
   if(enabledFeatures & FEATURE_HYBRID_CPU){
      SysSetTrapAddress(sysTrapPceNativeCall, &emuPceNativeCall);
   }
}

static void setArmStack(){
   if(configFile[ARM_STACK_SIZE] > 0){
      uint8_t* armStack = MemPtrNew(configFile[ARM_STACK_SIZE]);/*this pointer never gets freed*/
      
      writeArbitraryMemory32(EMU_REG_ADDR(EMU_VALUE), (uint32_t)armStack);
      writeArbitraryMemory32(EMU_REG_ADDR(EMU_CMD), CMD_SET_ARM_STACK);
   }
}

void setConfigDefaults(){
   configFile[ARM_STACK_SIZE] = 0x4000;
   configFile[LCD_WIDTH] = 160;
   configFile[LCD_HEIGHT] = 220;
}

void showGui(){
   //TODO
}

DWord PilotMain(Word cmd, Ptr cmdBPB, Word launchFlags){
   DmOpenRef configDb;
   MemHandle configHandle;
   
   configDb = DmOpenDatabaseByTypeCreator('EMUC', 'GuiC', dmModeReadOnly);
   
   /*create db and set defaults if config doesnt exist*/
   if(!configDb){
      DmCreateDatabase(0/*cardNo*/, 'Emu Config', 'GuiC', 'EMUC', true);
      configDb = DmOpenDatabaseByTypeCreator('EMUC', 'GuiC', dmModeReadOnly);
      configHandle = DmNewResource(configDb, 'CONF', 0, CONFIG_FILE_SIZE);
      configFile = MemHandleLock(configHandle);
      setConfigDefaults();
   }
   else{
      configHandle = DmGet1Resource('CONF', 0);
      configFile = MemHandleLock(configHandle);
   }
   
   if(cmd == sysAppLaunchCmdNormalLaunch)
      showGui();
   else if(cmd == sysAppLaunchCmdSystemReset){
      installTrapHandlers();
      setArmStack();
   }
   
   MemHandleUnlock(configHandle);
   DmReleaseResource(configHandle);
   DmCloseDatabase(configDb);
   
   return 0;
}
