testStatus: Test/testStatus.cc Include/Status.h Util/Status.cc
	g++ Test/testStatus.cc Include/Status.h Util/Status.cc -o TestStatus

testWrite:  Test/testWriteLog.cc Include/Status.h Util/Status.cc Util/Crc32c.h Util/Crc32c.cc  Include/Env.h Include/Env1.h  DB/Log_format.h DB/log_writer.h DB/log_writer.cc Include/Slice.h 	
	g++ Test/testWriteLog.cc Include/Status.h Util/Status.cc Util/Crc32c.h Util/Crc32c.cc  Include/Env.h Include/Env1.h  DB/Log_format.h DB/log_writer.h DB/log_writer.cc Include/Slice.h -g -o testWrite

testCom:     Include/Comparator.h  Test/testComparator.cc Include/Slice.h Util/Comparator.cc 
	g++  Include/Comparator.h  Test/testComparator.cc Include/Slice.h Util/Comparator.cc    -g -o testCom 


testSkipList:  DB/SkipList.h Util/Arena.h Test/testskiplist.cc 
	g++    DB/SkipList.h Util/Arena.h Test/testskiplist.cc  -g -o testSkipList

testMemtable: DB/Memtable.h Util/Comparator.cc Test/testMemtable 
	g++    DB/Memtable.h Util/Comparator.cc Test/testMemtable -g -o testMemtable


clean:    
	rm testSkipList



