
CC=g++
#CFLAGS=-g -Wall -m32
CFLAGS=-g -Wall
LIBS=-lmosquitto
DEFINE_FLAGS=-D_PC_
DEFINE_FLAGS+=-DPRINT
#DEFINE_FLAGS+=-DDEBUG_CRC
DEFINE_FLAGS+=-DDEBUG_SYNC
#DEFINE_FLAGS+=-DDEBUG_USART_READ
#DEFINE_FLAGS+=-DDEBUG_USART_WRITE

HNSHUBLIB = HNSSocketPower

#////////////////////////////////////////////////////////////////////////

INCLUDE_PATH = ../lib/HNSCommand ../lib/HNSCore ../lib/HNSHub

HNSHUB_PATH_DIR = ../lib/HNSHub

HNSHUBLIB_PATH_DIR = ../lib/HNSHubLib
HNSHUBLIB_PATHS = $(patsubst %, $(HNSHUBLIB_PATH_DIR)/%, $(HNSHUBLIB))

INCLUDE_PATH += $(HNSHUBLIB_PATHS)

_INCL = $(patsubst %, -I%, $(INCLUDE_PATH))

EXEC = main

$(EXEC): ../src/$(EXEC).cpp HNSSocketPower.o libHNSHub.a libHNSCore.a 
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -o $@ $^ $(_INCL) $(LIBS) 
	echo $(_INCL)


#////////////////////////////libHNSSocketPower////////////////////////////////////////
_OBJ_ = HNSSocketPower
_BASIC_DEPS_ = $(patsubst %, $(HNSHUBLIB_PATH_DIR)/$(_OBJ_)/$(_OBJ_).cpp, $(_OBJ_))
_BASIC_DEPS_ += $(patsubst %, $(HNSHUBLIB_PATH_DIR)/$(_OBJ_)/$(_OBJ_).h, $(_OBJ_))

$(_OBJ_).o: $(_BASIC_DEPS_) libHNSHub.a ../lib/HNSCommand/HNSSocketV2.h  
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)
#////////////////////////////END libHNSSocketPower////////////////////////////////////////



#////////////////////////////libHNSHub////////////////////////////////////////
libHNSHub.a: HNSHubInfo.o sigint_handler.o HNSRefresh.o HNSAddressMap.o HNSDatabase.o HNSDevice_class.o HNSMQTT.o HNSSync.o
	ar rvs libHNSHub.a HNSHubInfo.o sigint_handler.o HNSRefresh.o HNSAddressMap.o HNSDatabase.o HNSDevice_class.o HNSMQTT.o HNSSync.o

HNSHubInfo.o: $(HNSHUB_PATH_DIR)/HNSHubInfo.cpp $(HNSHUB_PATH_DIR)/HNSHubInfo.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)

sigint_handler.o: $(HNSHUB_PATH_DIR)/sigint_handler.c $(HNSHUB_PATH_DIR)/sigint_handler.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)

HNSRefresh.o: $(HNSHUB_PATH_DIR)/HNSRefresh.cpp $(HNSHUB_PATH_DIR)/HNSRefresh.hpp libHNSCore.a HNSDatabase.o
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $<  $(_INCL) $(LIBS)

HNSAddressMap.o: $(HNSHUB_PATH_DIR)/HNSAddressMap.cpp $(HNSHUB_PATH_DIR)/HNSAddressMap.hpp  
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)

HNSDatabase.o: $(HNSHUB_PATH_DIR)/HNSDatabase.cpp $(HNSHUB_PATH_DIR)/HNSDatabase.hpp  HNSAddressMap.o HNSSync.o libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)

HNSDevice_class.o: $(HNSHUB_PATH_DIR)/HNSDevice_class.cpp $(HNSHUB_PATH_DIR)/HNSDevice_class.hpp libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)

HNSMQTT.o: $(HNSHUB_PATH_DIR)/HNSMQTT.cpp $(HNSHUB_PATH_DIR)/HNSMQTT.hpp $(HNSHUB_PATH_DIR)/HNSUtil.hpp libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)

HNSSync.o: $(HNSHUB_PATH_DIR)/HNSSync.cpp $(HNSHUB_PATH_DIR)/HNSSync.hpp libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< $(_INCL) $(LIBS)
#////////////////////////////END libHNSHub////////////////////////////////////////



#////////////////////////////libHNSCore////////////////////////////////////////
libHNSCore.a: HNSTest.o HNSRouting.o CRC.o HNSError.o USART_Linux.o HNSCore.o
	ar rvs libHNSCore.a HNSTest.o HNSRouting.o CRC.o HNSError.o USART_Linux.o HNSCore.o

HNSCore.o: ../lib/HNSCore/HNSCore.h ../lib/HNSCore/HNSCore.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSCore.c $(LIBS)

HNSTest.o : HNSRouting.o  ../lib/HNSCore/HNSTest.h  ../lib/HNSCore/HNSTest.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSTest.c $(LIBS)

HNSRouting.o: USART_Linux.o HNSError.o CRC.o ../lib/HNSCore/HNSRouting.h ../lib/HNSCore/HNSRouting.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSRouting.c $(LIBS)

CRC.o: ../lib/util/CRC.c ../lib/util/CRC.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/util/CRC.c $(LIBS)

USART_Linux.o: ../lib/util/Linux/USART_Linux.h ../lib/util/Linux/USART_Linux.c ../lib/util/Linux/MAX485.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/util/Linux/USART_Linux.c ../lib/util/Linux/MAX485.h $(LIBS)

HNSError.o: ../lib/HNSCore/HNSError.h ../lib/HNSCore/HNSError.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSError.c $(LIBS)
#////////////////////////////END libHNSCore////////////////////////////////////////

.phony: clean

clean:
	rm -f *.o *.a $(EXEC)
