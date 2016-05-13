#include "log.h"
#include <string.h>

int main() {
	init_log("unittest");
	set_log_level(7);
	toggle_hex_level();

	log_debug("dubug......%d",10);
	log_info("info......%d",12345);
	log_notice("notice......%f",5.5F);
	log_warning("warning......%s","bbbbbbbbbbb");
	log_error("error......%ld",987654L);
	log_crit("crit......%lf",999.1);
	log_alert("alert......%u",111111U);
	log_emerg("emerg......%d",100);
	TRACE("TRACE......%d",1000);

	int s = 0xABCDEF;
	hex_debug("hex",sizeof s,(char*)&s);

	const char* str = "abcdefghijklmnopqrstuvwxyz";
	hex_debug("hex",strlen(str),(char*)str);
}

