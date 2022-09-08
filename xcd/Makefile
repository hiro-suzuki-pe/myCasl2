# (1)コンパイラ
CC  = gcc
YACC	= bison -d
LEX		= flex

# (2)コンパイルオプション
CFLAGS    = -w -g #  -Wall

# (3)実行ファイル名
TARGET  = xcd.exe

# (4)コンパイル対象のソースコード
SRCS    = xcd.c 

# (5)オブジェクトファイル名
OBJS    = $(SRCS:.c=.o)

# (6)インクルードファイルのあるディレクトリパス
INCDIR  = 

# (7)ライブラリファイルのあるディレクトリパス
LIBDIR  = 

# (8)追加するライブラリファイル
LIBS    = 	

# (9)ターゲットファイル生成
$(TARGET): $(OBJS)
	$(CC) -o $@ -g $^ $(LIBDIR) $(LIBS)
	$(TARGET)	../test.csx

# (10)オブジェクトファイル生成
.c.o:	
	$(CC) $(CFLAGS) $(INCDIR)	-c $<


# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)

# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	rm -f $(OBJS) $(TARGET) 
