COMPONENT=SenseAppC
BUILD_EXTRA_DEPS += SenseMsg.class 
CLEAN_EXTRA = *.class SenseMsg.java

SenseMsg.class: SenseMsg.java 
	javac	SenseMsg.java 
 
SenseMsg.java: 
	mig java -target=$(PLATFORM) -java-classname=SenseMsg Sense.h SenseMsg -o $@ 
 
include $(MAKERULES)