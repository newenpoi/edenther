// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <locale.h>
#include "UIWindow.h"

int main(int, char**)
{
    // Définit les paramètres régionaux et donc la page de codes ANSI par défaut de l’utilisateur pour ce pays/cette région, telle qu’elle est obtenue à partir du système d’exploitation hôte.
    _wsetlocale(LC_ALL, L"fr-FR");

    // Lance l'application uniquement si l'initialisation a réussie.
    if (UIWINDOW.Init(1, 1, 1, 1)) UIWINDOW.Run();

    return 0;
}
