#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define MX 101

using namespace std;

class MatriceRara {
private:
    int nrlinii, nrcol, nrnenule;
    int linii[MX] = { 0 }, col[MX] = { 0 };
    double val[MX] = { 0 };
public:
    explicit MatriceRara(const int n = 0, const int m = 0) : nrlinii(n), nrcol(m), nrnenule(0) {
        for (int i = 1; i < MX; i++) {
            linii[i] = 0;
            col[i] = 0;
            val[i] = 0.0;
        }
    }

    MatriceRara(const int n, const int m, const int w) : nrlinii(n), nrcol(m), nrnenule(w) {
        srand(time(nullptr));
        int incercari = 0;
        const int Maxincercari = w * 100;
        while (incercari < Maxincercari) {
            incercari++;
            for (int i = 1; i <= w; ++i) {
                const int x = rand() % nrlinii + 1;
                const int y = rand() % nrcol + 1;
                const double z = rand() % 100 + 1;

                const double z1 = getelement(*this, x, y);
                if (z1 == 0) {
                    linii[i] = x;
                    col[i] = y;
                    val[i] = static_cast<double>(z);
                }
                else continue;
            }
        }
    }
    MatriceRara( MatriceRara& M1) : nrlinii(M1.nrlinii), nrcol(M1.nrcol), nrnenule(M1.nrnenule) {
        for (int i = 1; i <= nrnenule; i++) {
            linii[i] = M1.linii[i];
            col[i] = M1.col[i];
            val[i] = M1.val[i];
        }
    }
    ~MatriceRara() = default;

    int getnrlinii() const { return nrlinii; }
    int getnrcol() const { return nrcol; }
    int getnrnenule() const { return nrnenule; }

    int getlin(int x) const { return linii[x]; }
    int getcol(int x) const { return col[x]; }
    int getval(int x) const { return val[x]; }

    friend double maxim( MatriceRara& M1);
    friend double minim( MatriceRara& M1);

    friend double getelement( MatriceRara& M1, const int L, const int C);
    friend void adaugaelement(MatriceRara& M1, const int l, const int c, const double v);

    friend MatriceRara operator+( MatriceRara& M1,  MatriceRara& M2);
    friend MatriceRara operator-( MatriceRara& M1,  MatriceRara& M2);

    friend MatriceRara operator*( MatriceRara& M1, const double n);
    friend MatriceRara operator*( const double n, MatriceRara& M1);

    friend istream& operator>>(istream& is, MatriceRara& M1);
    friend ostream& operator<<(ostream& os, const MatriceRara& M1);

};

///------------------------AICI SE TERMINA CLASA--------------------------------------

double maxim( MatriceRara& M1) {
    double maxi = M1.val[1];
    for (int i = 2; i <= M1.nrnenule; i++)
        if (M1.val[i] > maxi) maxi = M1.val[i];
    return maxi;
}

double minim( MatriceRara& M1) {
    double mini = M1.val[1];
    for (int i = 2; i <= M1.nrnenule; i++)
        if (M1.val[i] < mini) mini = M1.val[i];
    return mini;
}


double getelement( MatriceRara& M1, const int L, const int C) {
    for (int i = 1; i <= M1.nrnenule; i++)
        if (M1.linii[i] == L && M1.col[i] == C) return M1.val[i];
    return 0.0;
}

void adaugaelement(MatriceRara& M1, const int l, const int c, const double v) {
    if (v == 0.0) {
        return;
    }
    if (l < 1 || l > M1.nrlinii || c < 1 || c > M1.nrcol)
        throw out_of_range("Indicii depasesc dimensiunile matricei");
    for (int i = 1; i <= M1.nrnenule; i++)
        if (M1.linii[i] == l && M1.col[i] == c) {
            M1.val[i] = v;
            return;
        }
    M1.nrnenule++;
    M1.linii[M1.nrnenule] = l;
    M1.col[M1.nrnenule] = c;
    M1.val[M1.nrnenule] = v;
    return;
}

MatriceRara operator+( MatriceRara& M1,  MatriceRara& M2) {
    if (M1.nrlinii != M2.nrlinii || M1.nrcol != M2.nrcol)
        throw runtime_error("Adunarea nu are sens in acest context (Matricele nu au aceleasi dimensiuni)");
    MatriceRara& rez(M1);
    for (int i = 1; i <= M2.nrnenule; i++) {
        double existent = getelement(rez, M2.linii[i], M2.col[i]);
        double suma = existent + M2.val[i];
        if (suma != 0.0) adaugaelement(rez, M2.linii[i], M2.col[i], suma);
    }
    return rez;
}

MatriceRara operator-( MatriceRara& M1,  MatriceRara& M2)
{
    if (M1.nrlinii != M2.nrlinii || M1.nrcol != M2.nrcol)
        throw runtime_error("Scaderea nu are sens in acest context (Matricele nu au aceleasi dimensiuni)");
    MatriceRara rez(M1);
    for (int i = 1; i <= M2.nrnenule; i++) {
        double existent = getelement(rez, M2.linii[i], M2.col[i]);
        double dif = existent - M2.val[i];
        if (dif != 0.0) adaugaelement(rez, M2.linii[i], M2.col[i], dif);
        else {
            for (int j = 1; j <= rez.nrnenule; j++)
                if (rez.linii[j] == M2.linii[i] && rez.col[j] == M2.col[i]) {
                    for (int k = 1; k < rez.nrnenule; k++) {
                        rez.linii[k] = rez.linii[k + 1];
                        rez.col[k] = rez.col[k + 1];
                        rez.val[k] = rez.val[k + 1];
                    }
                    --rez.nrnenule;
                    break;
                }
        }
    }
    return rez;
}

MatriceRara operator*( MatriceRara& M1, const double n) {
    MatriceRara& rez(M1);
    if (n == 0.0) return rez;
    for (int i = 1; i <= M1.nrnenule; i++) {
        adaugaelement(rez, M1.linii[i], M1.col[i], M1.val[i] * n);
    }
    return rez;
}

MatriceRara operator*(const double n, MatriceRara& M1) {
    MatriceRara& rez(M1);
    if (n == 0.0) return rez;
    for (int i = 1; i <= M1.nrnenule; i++) {
        adaugaelement(rez, M1.linii[i], M1.col[i], n * M1.val[i]);
    }
    return rez;
}

istream& operator>>(istream& is, MatriceRara& M1) {
    for (int i = 1; i <= M1.nrnenule; i++) {
        M1.linii[i] = 0;
        M1.col[i] = 0;
        M1.val[i] = 0.0;
    }
    is >> M1.nrlinii >> M1.nrcol >> M1.nrnenule;
    for (int i = 1; i <= M1.nrnenule; i++) {
        int l, c; double v;
        is >> l >> c >> v;
        adaugaelement(M1, l, c, v);
    }
    return is;
}
ostream& operator<<(ostream& os, const MatriceRara& M1) {
    os << "Numarul de linii: " << M1.nrlinii << endl;
    os << "Numarul de coloane: " << M1.nrcol << endl;
    os << "Numarul de elemente nenule: " << M1.nrnenule << endl;

    for (int i = 1; i <= M1.nrnenule; i++) {
        os << "linii[" << i << "]: " << M1.linii[i] << " ";
        os << "col[" << i << "]: " << M1.col[i] << " ";
        os << "val[" << i << "]: " << M1.val[i] << endl;
    }
    return os;
}

///------------------------AICI SE TERMINA FUNCTIILE CLASEI--------------------------------------



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	glClearColor(0.8f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}

int main() {

    MatriceRara Matrice[50];

    ///Matricele de afisare
    MatriceRara MatInmultire;
    MatriceRara MatAdunare;
    MatriceRara MatScadere;

    int nrMat = 0, test = 0;
    int MatSelAdaugare = 0;
    char Triplete[250] = "";
    int ColSel = 0;
    int LinSel = 0;
    double ValSel = 0;
    int ColCitSel = 0;
    int LinCitSel = 0;
    int ValCitSel = 0;
    int MatSelCitire = 0;
    int MatSelCopiere1 = 0;
    int MatSelCopiere2 = 0;
    int MatSelMin = 0;
    int MatSelMax = 0;
    int MatSelAdunare1 = 0;
    int MatSelAdunare2 = 0;
    int MatSelScadere1 = 0;
    int MatSelScadere2 = 0;
    double ScalSel = 0;
    int MatSelInmultire = 0;
    int MatSelStergere = 0;

    /// Butoane de pop-up 

    bool CitirePopup = 0;
    bool CopierePopup = 0;
    bool AdaugarePopup = 0;
    bool MinimPopup = 0;
    bool MaximPopup = 0;
    bool SumaPopup = 0;
    bool ScaderePopup = 0;
    bool InmultirePopup = 0;
    bool StergerePopup = 0;
    bool FormaCondensata = 0;
    bool AfisCitire = 0;
    bool AfisInmult = 0; 
    bool AfisAdunare = 0;
    bool AfisScadere = 0;
    

	glfwInit();

	/// Setting up the window 

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Pisictor", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


	/// Render loop functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int selectedIndex = 1;
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.8f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();


        /// Interfata 
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::BeginChild("Operatii", ImVec2(400, 0), true);
            ImGui::SetWindowFontScale(2.0f);
            ImGui::SeparatorText("Operatii");

            ImGui::SetWindowFontScale(1.5f);
            ImGui::SeparatorText("Atribuire");
                if (ImGui::Button("Creare Matrice goala")) {
                    nrMat++;
                };
                if (ImGui::Button("Creare Matrice random")) {
                    MatriceRara Aux(10, 10, 5);
                    cout << Aux;
                    nrMat++;
                    Matrice[nrMat] = Aux;
                };
                if (ImGui::Button("Citire")) {
                    CitirePopup = 1;
                };
                if (ImGui::Button("Copiere")) {
                    CopierePopup = 1;
                };
                if (ImGui::Button("Adaugare Element")) {
                    AdaugarePopup = 1;
                };
            ImGui::SeparatorText("Cautare");
                if (ImGui::Button("Valoare Minima")) {
                    MinimPopup = 1;
                };
                if (ImGui::Button("Valoare Maxima")) {
                    MaximPopup = 1;
                };
            ImGui::SeparatorText("Aritmetica");
                if (ImGui::Button("Adunare Matrice")) {
                    SumaPopup = 1;
                };
                if (ImGui::Button("Scadere Matrice")) {
                    ScaderePopup = 1;
                };
                if (ImGui::Button("Inmultire cu un scalar")) {
                    InmultirePopup = 1;
                };
            ImGui::SeparatorText("Afisare");
                if(!FormaCondensata)
                    if (ImGui::Button("Afisare in forma condensata")) 
                        FormaCondensata = 1;
                if (FormaCondensata)
                    if (ImGui::Button("Afisare in forma extinsa"))
                        FormaCondensata = 0;
             ImGui::SeparatorText("Stergere");
                if (ImGui::Button("Stergere Matrice")) {
                    StergerePopup = 1;
                };
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Matrice", ImVec2(600, 0), true);
        ImGui::SetWindowFontScale(2.0f);
            ImGui::SeparatorText("Matrice");
            for (int i = 1; i <= nrMat; i++) {
                ImGui::SetWindowFontScale(1.5f);
                ImGui::Text("Matricea %d", i);
                ImGui::SetWindowFontScale(1.2f);
                ImGui::Text("Nr linii %d", Matrice[i].getnrlinii());
                ImGui::Text("Nr Coloane %d", Matrice[i].getnrcol());
                ImGui::Text("Nr Valori Nenule %d", Matrice[i].getnrnenule());
                ImGui::NewLine();
                if (FormaCondensata) {
                    for (int j = 1; j <= Matrice[i].getnrnenule(); j++) {
                        ImGui::Text("%d %d %d", Matrice[i].getlin(j), Matrice[i].getcol(j), Matrice[i].getval(j));
                    }        
                }
                else {
                    for (int j = 1; j <= Matrice[i].getnrlinii(); j++) {
                        for (int k = 1; k <= Matrice[i].getnrcol(); k++) {
                            int aux = getelement(Matrice[i], j, k);
                            if(!aux)
                                ImGui::Text("%d", aux);
                            else
                                ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "%d", aux);
                            ImGui::SameLine();
                        }
                        ImGui::NewLine();
                    }
                }
                ImGui::SeparatorText("");
            }
        ImGui::EndChild();

        ImGui::SameLine();

        if (CitirePopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
            ImGui::Begin("Citire", &CitirePopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Matricea selectata", &MatSelCitire, 0, 0);;
            ImGui::SetWindowFontScale(1.2f);
            ImGui::NewLine();
            ImGui::Text("(Valorile vechi ale matricei vor fi suprascrise!)");
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Nr de Linii", &LinCitSel, 0, 0);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Nr de Coloane", &ColCitSel, 0, 0);
            ImGui::SetNextItemWidth(60.0f);
            if (MatSelCitire <= nrMat && MatSelCitire >= 1 && LinCitSel > 0 && LinCitSel < 101 && ColCitSel > 0 && ColCitSel < 101){
                    if (ImGui::Button("Confirma parametrii matricei")) {
                        AfisCitire = 1;
                        MatriceRara Aux(LinCitSel, ColCitSel);
                        Matrice[MatSelCitire] = Aux;
                    }
                    if (AfisCitire) {
                        ImGui::NewLine();
                        ImGui::SameLine();    
                        ImGui::SetNextItemWidth(80.0f);
                        ImGui::SetWindowFontScale(1.2f);
                        ImGui::InputText("(Linie - Coloana - Valoare)",Triplete, sizeof(Triplete));
                        ImGui::SetWindowFontScale(1.8f);
                           if (ImGui::Button("Adauga Valoarea")) {
                               stringstream ss(Triplete);
                               int a = 0, b = 0, c = 0;
                               ss >> a >> b >> c;
                               cout << a << b << c;
                               adaugaelement(Matrice[MatSelCitire], a, b, (double)c);

                           }
                        
                    }
            }

            ImGui::End();
        }

        if (CopierePopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_Once);
            ImGui::Begin("Copiere", &CopierePopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Matricea target", &MatSelCopiere1, 0, 0);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Matricea scope", &MatSelCopiere2, 0, 0);
            if (MatSelCopiere1 <= nrMat && MatSelCopiere1 >= 1 && MatSelCopiere2 <= nrMat && MatSelCopiere2 >= 1) {
                    if (ImGui::Button("Confirma")) {
                        Matrice[MatSelCopiere1] = Matrice[MatSelCopiere2];
                    }
            }

            ImGui::End();
        }

        if (AdaugarePopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_Once);
            ImGui::Begin("Adaugare", &AdaugarePopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Matricea selectata", &MatSelAdaugare, 0, 0);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Coloana selectata", &ColSel, 0, 0);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Linia selectata", &LinSel, 0, 0);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputDouble("Valoarea selectata", &ValSel, 0, 0);
            if (MatSelAdaugare <= nrMat && MatSelAdaugare >= 1) {
                if(ColSel>=1 && ColSel<=Matrice[MatSelAdaugare].getnrcol() && LinSel >=1 && LinSel <=Matrice[MatSelAdaugare].getnrlinii())
                    if (ImGui::Button("Confirma")) {
                        adaugaelement(Matrice[MatSelAdaugare], LinSel, ColSel, ValSel);
                    }
            }

            ImGui::End();
        }

        if (MinimPopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_Once);
            ImGui::Begin("Minim", &MinimPopup, ImGuiWindowFlags_NoResize);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::SetNextItemWidth(60.0f);
                ImGui::InputInt("Matricea selectata", &MatSelMin, 0, 0);
                if (MatSelMin <= nrMat && MatSelMin >= 1) {
                    ImGui::NewLine();
                    ImGui::Text("Valoarea minima din matrice: %d", (int)minim(Matrice[MatSelMin]));
                }

            ImGui::End();
        }

        if (MaximPopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_Once);
            ImGui::Begin("Maxim", &MaximPopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Matricea selectata", &MatSelMax, 0, 0);
            if (MatSelMax <= nrMat && MatSelMax >= 1) {
                ImGui::NewLine();
                ImGui::Text("Valoarea maxima din matrice: %d", (int)maxim(Matrice[MatSelMax]));
            }

            ImGui::End();
        }

        if (SumaPopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
            ImGui::Begin("Suma", &SumaPopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Prima matrice selectata", &MatSelAdunare1, 0, 0);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("A doua matrice selectata", &MatSelAdunare2, 0, 0);
            if (MatSelAdunare1 <= nrMat && MatSelAdunare1 >= 1 && MatSelAdunare2 <= nrMat && MatSelAdunare2 >=1 ) {
                MatriceRara Aux;
                ImGui::NewLine();
                if (ImGui::Button("Confirma")) {
                    AfisAdunare = 1;
                    Aux = Matrice[MatSelAdunare1];
                    Aux = Aux + Matrice[MatSelAdunare2];
                    MatAdunare = Aux;

                }
                if (AfisAdunare) {
                    ImGui::NewLine();
                    if (FormaCondensata)
                        for (int j = 1; j <= MatAdunare.getnrnenule(); j++)
                            ImGui::Text("%d %d %d", MatAdunare.getlin(j), MatAdunare.getcol(j), MatAdunare.getval(j));
                    else {
                        for (int j = 1; j <= MatAdunare.getnrlinii(); j++) {
                            for (int k = 1; k <= MatAdunare.getnrcol(); k++) {
                                int aux = getelement(MatAdunare, j, k);
                                if (!aux)
                                    ImGui::Text("%d", aux);
                                else
                                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "%d", aux);
                                ImGui::SameLine();
                            }
                            ImGui::NewLine();
                        }

                    }
                    ImGui::NewLine();
                    if (ImGui::Button("Initializeaza matricea")) {
                        nrMat++;
                        Matrice[nrMat] = MatAdunare;
                    }
                }
            }

            ImGui::End();
        }
        
        if (ScaderePopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
            ImGui::Begin("Scadere", &ScaderePopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Prima matrice selectata", &MatSelScadere1, 0, 0);
            ImGui::SetNextItemWidth(100.0f);
            ImGui::InputInt("A doua matrice selectata", &MatSelScadere2, 0, 0);
            if (MatSelScadere1 <= nrMat && MatSelScadere1 >= 1 && MatSelScadere2 <= nrMat && MatSelScadere2 >= 1) {
                MatriceRara Aux;
                ImGui::NewLine();
                if (ImGui::Button("Confirma")) {
                    AfisScadere = 1;
                    Aux = Matrice[MatSelScadere1];
                    Aux = Aux - Matrice[MatSelScadere2];
                    MatScadere = Aux;

                }
                if (AfisScadere) {
                    ImGui::NewLine();
                    if (FormaCondensata)
                        for (int j = 1; j <= MatScadere.getnrnenule(); j++)
                            ImGui::Text("%d %d %d", MatScadere.getlin(j), MatScadere.getcol(j), MatScadere.getval(j));
                    else {
                        for (int j = 1; j <= MatScadere.getnrlinii(); j++) {
                            for (int k = 1; k <= MatScadere.getnrcol(); k++) {
                                int aux = getelement(MatScadere, j, k);
                                if (!aux)
                                    ImGui::Text("%d", aux);
                                else
                                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "%d", aux);
                                ImGui::SameLine();
                            }
                            ImGui::NewLine();
                        }

                    }
                    ImGui::NewLine();
                    if (ImGui::Button("Initializeaza matricea")) {
                        nrMat++;
                        Matrice[nrMat] = MatScadere;
                    }
                }
            }

            ImGui::End();
        }

        if (InmultirePopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
            ImGui::Begin("Inmultire", &InmultirePopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Matricea selectata", &MatSelInmultire, 0, 0);
            ImGui::SetNextItemWidth(100.0f);
            ImGui::InputDouble("Scalarul selectat", &ScalSel, 0, 0);
            if (MatSelInmultire <= nrMat && MatSelInmultire >= 1) {
                MatriceRara Aux;
                ImGui::NewLine();
                if (ImGui::Button("Confirma")) {
                    AfisInmult = 1;
                    Aux = Matrice[MatSelInmultire];
                    Aux = Aux * ScalSel;
                    MatInmultire = Aux;
                    
                }
                if (AfisInmult) {
                    ImGui::NewLine();
                    if (FormaCondensata)
                        for (int j = 1; j <= MatInmultire.getnrnenule(); j++)
                            ImGui::Text("%d %d %d", MatInmultire.getlin(j), MatInmultire.getcol(j), MatInmultire.getval(j));
                    else {
                        for (int j = 1; j <= MatInmultire.getnrlinii(); j++) {
                            for (int k = 1; k <= MatInmultire.getnrcol(); k++) {
                                int aux = getelement(MatInmultire, j, k);
                                if (!aux)
                                    ImGui::Text("%d", aux);
                                else
                                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "%d", aux);
                                ImGui::SameLine();
                            }
                            ImGui::NewLine();
                        }

                    }
                    ImGui::NewLine();
                    if (ImGui::Button("Initializeaza matricea")) {
                        nrMat++;
                        Matrice[nrMat] = MatInmultire;
                    }
                }
            }

            ImGui::End();
        }

        if (StergerePopup) {
            ImGui::SetNextWindowPos(ImVec2(800, 100), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
            ImGui::Begin("Stergere", &StergerePopup, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetNextItemWidth(60.0f);
            ImGui::InputInt("Matricea selectata", &MatSelStergere, 0, 0);
            if (MatSelStergere <= nrMat && MatSelStergere >= 1) {
                ImGui::NewLine();
                if (ImGui::Button("Confirma")) {
                    for (int i = MatSelStergere; i < nrMat; i++)
                        Matrice[i] = Matrice[i + 1];
                    nrMat--;

                }
            }

            ImGui::End();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
