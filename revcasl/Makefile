# (1)コンパイラ
CC  = gcc
YACC	= bison -d
LEX		= flex

# (2)コンパイルオプション
CFLAGS    = -w -g 

# (3)実行ファイル名
TARGET  = revcasl.exe

# (4)コンパイル対象のソースコード
SRCS    = revcasl.c snapshot.c

# (5)オブジェクトファイル名
OBJS    = $(SRCS:.c=.o)

# (6)インクルードファイルのあるディレクトリパス
INCDIR  = 	-I..\casl		#		

# (7)ライブラリファイルのあるディレクトリパス
LIBDIR  = 

# (8)追加するライブラリファイル
LIBS    = 	 			# -lfl  # 	-ll

# (9)ターゲットファイル生成
$(TARGET): $(OBJS)
	$(CC)  -g -o $@ $^ $(LIBDIR) $(LIBS)
	$(TARGET) ..\\test.csx

# (10)オブジェクトファイル生成
.c.o:	
	$(CC) $(CFLAGS) $(INCDIR)	-c $<

# (11)"make all"で make cleanとmakeを同時に実施。
all: clean $(OBJS) $(TARGET)

# (12).oファイル、実行ファイル、.dファイルを削除
clean:
	del /q $(OBJS) $(TARGET) 
