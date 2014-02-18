# 実行可能ファイル
TARGET = a.mines

# オブジェクトファイル群
OBJS = main.o mainwin.o setup.o dialog.o mybutton.o

# C++ コンパイラ
CXX = g++

# オブジェクトファイル生成時のコンパイルオプション
CFLAGS = -c `pkg-config gtkmm-2.4 --cflags`

# リンク時のコンパイルオプション
LFLAGS = `pkg-config gtkmm-2.4 --libs`

# 実行可能ファイルの生成
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LFLAGS) 

# オブジェクトファイル生成記述に使うサフィックスルール
# .oというファイルが必要になれば，これを.cppからつくる というルール
.SUFFIXES: .cpp .o

# オブジェクトファイルの生成
.cpp.o:
	$(CXX) $< $(CFLAGS)

# 中間生成物削除：オブジェクトファイル群の削除
clean:
	rm -f $(OBJS)

# コンパイル結果削除：オブジェクトファイルと実行可能ファイルの削除
cleanall:
	rm -f $(OBJS) $(TARGET)

# 依存関係の記述
main.o:mines.h main.cpp
mainwin.o:mines.h mainwin.cpp
setup.o:mines.h setup.cpp
dialog.o:mines.h dialog.cpp
mybutton.o:mines.h mybutton.cpp
a.mines:main.o mainwin.o setup.o dialog.o mybutton.o