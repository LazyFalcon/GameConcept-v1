DIRECTORIES = $(addprefix -I./,$(shell ls -d ./src/*/))
DIRECTORIES_2 = $(addprefix -I./,$(shell ls -d ./src/*/*/))
TARGET_NAME = TDv4_0_2

DEFINES = \
-DGLM_ENABLE_EXPERIMENTAL \
-DGLM_FORCE_SWIZZLE \
-DGLM_SWIZZLE \
-DGLM_FORCE_RADIANS
# -DUSE_BULLET
# -DBT_USE_DOUBLE_PRECISION=ON

CORE_PCH_FILENAME=./src/core_pch.hpp
CORE_PCH=$(CORE_PCH_FILENAME).gch

CXX_FLAGS = -isystem C:\MinGW\include -std=c++17 -O2 -msse2 -mfpmath=sse -g -pipe -I. -I./src $(DIRECTORIES) $(DIRECTORIES_2) $(DEFINES)
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
ADDITIONAL_FLAGS = \
-Werror=return-type \
-Werror=reorder \
-Wmaybe-uninitialized \
-Winvalid-pch
# -Wall
# -Wunused-function \
# -Wswitch-enum \

CXX = C:\MinGw\bin\g++.exe
FFMPEG = C:\ffmpeg\bin\ffmpeg.exe
SRC = ./src
BIN = ./bin
OBJ_DIR = ./obj

LIBS = -lPMK-audio -lboost_system -lboost_filesystem -lboost_thread -lboost_date_time -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath \
-lglfw3 -lgdi32 -lglu32 -lopengl32 -lassimp.dll -lFreeImage -lFreeImagePlus -lpng -ljpeg -lz -lOpenAL32 -lvorbisfile  -lvorbis -logg -lvorbisenc -lFLAC
# -lglew32

TARGETS = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)

OBJS = $(TARGETS:%.cpp=$(OBJ_DIR)/%.o)

DEP = $(OBJS:%.o=%.d)
# include($(shell find . -type f -name \*.d))

$(BIN)/$(TARGET_NAME): $(OBJS) ./obj/res.o
	@mkdir -p ./bin
	@echo "Linking: $@"
	@$(CXX) $^ -o $@ $(LIBS)
	@echo "Done"

$(CORE_PCH):
	@echo "Compiling PCH"
	@$(CXX) $(CXX_FLAGS) $(ADDITIONAL_FLAGS) $(CORE_PCH_FILENAME)

# -ftime-report
#	@powershell -c "Write-Host Compiling $< took: (Measure-Command { $(CXX) $(CXX_FLAGS) $(ADDITIONAL_F LAGS) -MMD -c $< -o $@ | Out-Default }).TotalSeconds"

-include $(DEP)

$(OBJ_DIR)/%.o : %.cpp
	@echo "Compiling: $< "
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) $(ADDITIONAL_FLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/res.o: ./resource.rc ./icon.ico
	windres ./resource.rc ./obj/res.o

pch:
	@echo "Compiling PCH"
	@$(CXX) $(CXX_FLAGS) $(ADDITIONAL_FLAGS) $(CORE_PCH_FILENAME)

clean:
	rm -rf $(OBJ_DIR)
	rm $(BIN)/$(TARGET_NAME).exe

run: $(BIN)/$(TARGET_NAME)
	$(BIN)/$(TARGET_NAME).exe

debug: $(BIN)/$(TARGET_NAME)
	gdb $(BIN)/$(TARGET_NAME).exe -ex=run

.PHONY: clean
