testStatus: Test/testStatus.cc Include/Status.h Util/Status.cc
	g++ Test/testStatus.cc Include/Status.h Util/Status.cc -o TestStatus

testWrite:  Test/testWriteLog.cc Include/Status.h Util/Status.cc Include/Env.h Include/Env1.h  DB/Log_format.h DB/log_writer.h DB/log_writer.cc Include/Slice.h 	
	g++ Test/testWriteLog.cc Include/Status.h Util/Status.cc Include/Env.h Include/Env1.h  DB/Log_format.h DB/log_writer.h DB/log_writer.cc Include/Slice.h -g  -o testWrite



