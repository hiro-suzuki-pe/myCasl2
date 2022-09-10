# (1)コンパイラ
CC  = gcc
AR  = ar
# (2)コンパイルオプション
CFLAGS = 
# (3)実行ファイル名
TARGET  = libUnity.a
# (4)コンパイル対象のソースコード
#SRCS = src\unity.c extras\fixture\src\unity_fixture.c extras\memory\test\unity_memory.c
SRC = 

# (5)オブジェクトファイル名
#OBJS    = $(SRCS:.c=.o)
OBJS = unity.o unity_fixture.o unity_memory.o

 
# (6)インクルードファイルのあるディレクトリパス
INCDIR = -Isrc -Iextras\fixture\src -Iextras\memory\src
 
# (7)ライブラリファイルのあるディレクトリパス
LIBDIR  = -L..\..\ApcControler_20200818\lib -L..\UnityHarness
 
# (8)追加するライブラリファイル
LIBS    = 

# (9)ターゲットファイル生成
$(TARGET): $(OBJS) 
	$(AR) r $@ $?
# (10)オブジェクトファイル生成
unity.o: src\unity.c 
	$(CC) $(CFLAGS) $(INCDIR)  -c $<

unity_fixture.o: extras\fixture\src\unity_fixture.c
	$(CC) $(CFLAGS) $(INCDIR)  -c $<

unity_memory.o: extras\memory\src\unity_memory.c
	$(CC) $(CFLAGS) $(INCDIR)  -c $<

# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)
# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	del $(OBJS) $(TARGET) 
