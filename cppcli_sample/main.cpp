#include <iostream>
#using <mscorlib.dll>

int main()
{
	//アンマネージ関数の呼び出し
	std::cout << "Stand by Ready!\n";

	//マネージ関数の呼び出し
	System::Console::WriteLine("Stand by Ready!");

	return 0;
}
