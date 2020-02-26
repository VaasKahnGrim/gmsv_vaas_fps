#include "GarrysMod/Lua/Interface.h"
// https://github.com/danielga/sourcesdk-minimal/ is required
#include "engine/iserverplugin.h"
#include "game/server/iplayerinfo.h"

#ifdef _LINUX
	#define SERVER_LIB "./garrysmod/bin/server_srv.so"
#else
	#define SERVER_LIB "server.dll"
#endif

#ifdef DEBUG
	#include "utilities.cpp"
#endif

CGlobalVars* g_GlobalVars = NULL;
IPlayerInfoManager* g_PlayerInfoManager = NULL;


using namespace GarrysMod::Lua;

LUA_FUNCTION(FrameCount)
{
	LUA->PushNumber((float)g_GlobalVars->framecount);
	return 1;
}

LUA_FUNCTION(RealFrameTime)
{
	LUA->PushNumber((float)g_GlobalVars->absoluteframetime);
	return 1;
}


void Vaas_LongMath_init(GarrysMod::Lua::ILuaBase* LUA){
	#ifdef DEBUG
		msg("Vaas LongMath module loaded.\n",0,255,0);
	#endif
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->GetField(-1,"engine");
		LUA->PushCFunction(FrameCount);
		LUA->SetField(-2,"FrameCount");

		LUA->PushCFunction(RealFrameTime);
		LUA->SetField(-2,"RealFrameTime");
	LUA->Pop(2);
}

// Called when you module is opened
GMOD_MODULE_OPEN(){
	CreateInterfaceFn serverFactory = Sys_GetFactory(SERVER_LIB);

	if (!serverFactory) {
		LUA->ThrowError("error getting serverFactory!! (basically fuck you from Garry)");
		return 0;
	}

	g_PlayerInfoManager = (IPlayerInfoManager*)serverFactory(INTERFACEVERSION_PLAYERINFOMANAGER, NULL);
	if (!g_PlayerInfoManager) {
		LUA->ThrowError("error getting g_PlayerInfoManager!! (still basically fuck you from Garry)");
		return 0;
	}

	g_GlobalVars = g_PlayerInfoManager->GetGlobalVars();

	Vaas_LongMath_init(LUA);
	return 0;
}

// Called when your module is closed
GMOD_MODULE_CLOSE(){
	#ifdef DEBUG
		msg("Vaas LongMath module nolonger in use, unloading.\n",0,255,0);
	#endif
	return 0;
}
