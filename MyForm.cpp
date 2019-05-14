#include "MyForm.h"
#include "class.h"
using namespace System; using namespace System;
using namespace System::Windows::Forms;
[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Проект4::MyForm form;
	Application::Run(%form);
}
