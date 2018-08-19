GCC=g++ 
GCC_INCLUDE = -I./src/QBSH_win32_vc2010/SDHumming -I./src/QBSH_win32_vc2010/SDFuzzySearch/ -I./src/QBSH_win32_vc2010/SDHBuildModel
GCC_OPTIONS = -g -c
LD=g++ 
all : tester.exe model.exe rank.exe

SDH_OBJ = ./src/QBSH_win32_vc2010/SDHumming/SDSP.o\
	./src/QBSH_win32_vc2010/SDHumming/SMelody.o\
	./src/QBSH_win32_vc2010/SDHumming/SUtil.o\
	./src/QBSH_win32_vc2010/SDHumming/SModel.o\
	./src/QBSH_win32_vc2010/SDFuzzySearch/SNoteMatch.o\
	./src/QBSH_win32_vc2010/SDFuzzySearch/SFrameMatch.o\
	./src/QBSH_win32_vc2010/SDFuzzySearch/SSearchCommon.o\
	./src/QBSH_win32_vc2010/SDFuzzySearch/SDFuzzySearch.o\
	#./midi-main.o
SDH_MODEL = ./src/QBSH_win32_vc2010/SDHBuildModel/midifile.o

STESTER_MAIN = ./src/QBSH_win32_vc2010/SDHumming/STester.o
SMODEL_MAIN  = ./src/QBSH_win32_vc2010/SDHBuildModel/SBuildModel.o
RANK_MAIN    = ./src/gurusangeetha/rank.o

tester.exe: $(SDH_OBJ) $(STESTER_MAIN)
	$(LD) $(STESTER_MAIN)  $(SDH_OBJ) -o $@

model.exe: $(SDH_OBJ) $(SDH_MODEL) $(SMODEL_MAIN)
	$(LD) $(SMODEL_MAIN)  $(SDH_OBJ) $(SDH_MODEL) -o $@

rank.exe: $(SDH_OBJ) $(SDH_MODEL) $(RANK_MAIN)
	$(LD) $(RANK_MAIN)  $(SDH_OBJ) $(SDH_MODEL) -o $@

%.o:%.cpp
	$(GCC) $(GCC_OPTIONS) $(GCC_INCLUDE) $< -o $@

%.o:%.c
	$(GCC) $(GCC_OPTIONS) $(GCC_INCLUDE) $< -o $@

clean:
	rm -rf $(SDH_OBJ) $(SDH_MODEL) $(STESTER_MAIN) $(SMODEL_MAIN) *.exe

