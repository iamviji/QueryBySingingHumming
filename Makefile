GCC=g++ 
GCC_INCLUDE = -I./src/QBSH_win32_vc2010/SDHumming -I./src/QBSH_win32_vc2010/SDFuzzySearch/ -I./src/QBSH_win32_vc2010/SDHBuildModel -I./src
GCC_OPTIONS = -g -c
LD=g++ 
AR=ar
AR_OPTIONS = -cr
all : tester.exe model.exe rank-qbh.exe rank-dtw.exe media2csv.exe

LIB_NAME=./src/similarity/libdtw.a

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
RANK_OBJ =  ./src/similarity/rank.o

STESTER_MAIN = ./src/QBSH_win32_vc2010/SDHumming/STester.o
SMODEL_MAIN  = ./src/QBSH_win32_vc2010/SDHBuildModel/SBuildModel.o
RANK_QBH_MAIN    = ./src/similarity/rank-qbh.o
RANK_DTW_MAIN    = ./src/similarity/rank-dtw.o
MEDIA2CSV_MAIN    = ./src/similarity/media2csv.o

tester.exe: $(SDH_OBJ) $(STESTER_MAIN)
	$(LD) $(STESTER_MAIN)  $(SDH_OBJ) -o $@

model.exe: $(SDH_OBJ) $(SDH_MODEL) $(SMODEL_MAIN)
	$(LD) $(SMODEL_MAIN)  $(SDH_OBJ) $(SDH_MODEL) -o $@

media2csv.exe: lib $(MEDIA2CSV_MAIN)
	$(LD) $(MEDIA2CSV_MAIN) $(LIB_NAME) -o $@

rank-qbh.exe: lib $(RANK_QBH_MAIN)
	$(LD) $(RANK_QBH_MAIN) $(LIB_NAME) -o $@

rank-dtw.exe: lib $(RANK_DTW_MAIN)
	$(LD) $(RANK_DTW_MAIN) $(LIB_NAME) -o $@

lib: $(SDH_OBJ) $(SDH_MODEL) $(RANK_OBJ)
	$(AR) $(AR_OPTIONS)  $(LIB_NAME) $(SDH_MODEL) $(SDH_OBJ) $(RANK_OBJ)

%.o:%.cpp
	$(GCC) $(GCC_OPTIONS) $(GCC_INCLUDE) $< -o $@

%.o:%.c
	$(GCC) $(GCC_OPTIONS) $(GCC_INCLUDE) $< -o $@

clean:
	rm -rf $(SDH_OBJ) $(SDH_MODEL) $(STESTER_MAIN) $(SMODEL_MAIN) *.exe $(LIB_NAME)

