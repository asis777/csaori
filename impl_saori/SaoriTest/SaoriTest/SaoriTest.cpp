#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#include "csaori.h"

/*---------------------------------------------------------
	初期化
---------------------------------------------------------*/
bool CSAORI::load(){
	return true;
}

/*---------------------------------------------------------
	解放
---------------------------------------------------------*/
bool CSAORI::unload(){
	return true;
}

/*---------------------------------------------------------
	実行
---------------------------------------------------------*/
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code = SAORIRESULT_OK;

	out.result = getModulePath();
	out.result += in.args[0];
	out.result += in.args[1];
}

