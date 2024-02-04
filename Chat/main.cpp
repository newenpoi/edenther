// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <locale.h>
#include "UIWindow.h"

int main(int, char**)
{
    // D�finit les param�tres r�gionaux et donc la page de codes ANSI par d�faut de l�utilisateur pour ce pays/cette r�gion, telle qu�elle est obtenue � partir du syst�me d�exploitation h�te.
    _wsetlocale(LC_ALL, L"fr-FR");

    // Lance l'application uniquement si l'initialisation a r�ussie.
    if (UIWINDOW.Init(1, 1, 1, 1)) UIWINDOW.Run();

    return 0;
}
