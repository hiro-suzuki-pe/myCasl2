# (1)コンパイラ
CC  = gcc
AR  = ar

# (2)コンパイルオプション
CFLAGS = -g
LFLAGS = -g

# (3)実行ファイル名
TARGET  = AllTests.exe
UNITY	= UnityHarness\libUnity.a

# (4)コンパイル対象のソースコード
SRCS = AllTests.c gccLibraryTest.c gccLibraryTestRunner.c

# (5)オブジェクトファイル名
OBJS    = $(SRCS:.c=.o)

 
# (6)インクルードファイルのあるディレクトリパス
INCDIR =  -IUnityHarness\src  -IUnityHarness\extras\fixture\src -IUnityHarness\extras\memory\src
 
# (7)ライブラリファイルのあるディレクトリパス
LIBDIR  = -LUnityHarness
 
# (8)追加するライブラリファイル
LIBS    = -lUnity

# (9)ターゲットファイル生成
$(TARGET): $(OBJS)
	$(CC)	$(LFLAGS) -o $@ $(OBJS) $(LIBDIR) $(LIBS)
	$(TARGET)

$(UNITY):
	$(MAKE) -C UnityHarness all

# (10)オブジェクトファイル生成
.c.o:
	$(CC) -c $(CFLAGS) $(INCDIR)	$<


# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(UNITY) $(TARGET) 

# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	del /q $(OBJS) $(TARGET) $(UNITY)