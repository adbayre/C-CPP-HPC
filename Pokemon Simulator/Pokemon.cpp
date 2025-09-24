#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <unordered_map>
#include <regex>
#include <iomanip>
using namespace std;

// Stocke la liste des types officiels
vector<string> listeTypes = {
    "Feu", "Eau", "Plante", "Électrik", "Glace", "Combat", "Poison", "Sol", "Vol",
    "Psy", "Insecte", "Roche", "Spectre", "Dragon","Normal", "Acier", "Fée"
};

// Map d'efficacité - faite d'après la matrice de la Gen 1, on a un peu triché pour les cas ou les attaques font x0
map<string, map<string, float>> mapTypes = {
    {"Normal",   {{"Roche", 0.5}, {"Spectre", 0.5}}},
    {"Feu",      {{"Feu", 0.5}, {"Eau", 0.5}, {"Plante", 2}, {"Glace", 2}, {"Insecte", 2}, {"Roche", 0.5}, {"Dragon", 0.5}}},
    {"Eau",      {{"Feu", 2}, {"Eau", 0.5}, {"Plante", 0.5}, {"Sol", 2}, {"Roche", 2}, {"Dragon", 0.5}}},
    {"Plante",   {{"Feu", 0.5}, {"Eau", 2}, {"Plante", 0.5}, {"Électrik", 0.5}, {"Glace", 0.5}, {"Sol", 2}, {"Roche", 2}, {"Vol", 0.5}, {"Dragon", 0.5}, {"Insecte", 0.5}}},
    {"Électrik", {{"Eau", 2}, {"Plante", 0.5}, {"Électrik", 0.5}, {"Sol", 0.5}, {"Vol", 2}, {"Dragon", 0.5}}},
    {"Glace",    {{"Feu", 0.5}, {"Eau", 0.5}, {"Plante", 2}, {"Glace", 0.5}, {"Sol", 2}, {"Vol", 2}, {"Dragon", 2}}},
    {"Combat",   {{"Normal", 2}, {"Glace", 2}, {"Roche", 2}, {"Spectre", 0.5}, {"Poison", 0.5}, {"Vol", 0.5}, {"Psy", 0.5}, {"Insecte", 0.5}}},
    {"Poison",   {{"Plante", 2}, {"Poison", 0.5}, {"Sol", 0.5}, {"Roche", 0.5}, {"Spectre", 0.5}}},
    {"Sol",      {{"Feu", 2}, {"Électrik", 2}, {"Plante", 0.5}, {"Poison", 2}, {"Vol", 0.5}, {"Roche", 2}}},
    {"Vol",      {{"Plante", 2}, {"Électrik", 0.5}, {"Insecte", 2}, {"Roche", 0.5}}},
    {"Psy",      {{"Combat", 2}, {"Poison", 2}, {"Psy", 0.5}}},
    {"Insecte",  {{"Feu", 0.5}, {"Plante", 2}, {"Combat", 0.5}, {"Poison", 0.5}, {"Vol", 0.5}, {"Spectre", 0.5}}},
    {"Roche",    {{"Feu", 2}, {"Glace", 2}, {"Combat", 0.5}, {"Sol", 0.5}, {"Vol", 2}, {"Insecte", 2}}},
    {"Spectre",  {{"Normal", 0.5}, {"Psy", 0.5}, {"Spectre", 2}}},
    {"Dragon",   {{"Dragon", 2}}},
    {"Acier",    {{"Acier", 0.5}, {"Dragon", 0.5}, {"Fée", 2}, {"Feu", 0.5}, {"Eau", 0.5}, {"Électrik", 0.5}, {"Glace", 2}, {"Roche", 2}}},
    {"Fée",      {{"Acier", 0.5}, {"Combat", 2}, {"Dragon", 2}, {"Feu", 0.5}, {"Poison", 0.5}, {"Ténèbres", 2}}}
};

class MapTypes {
    public:
        static float getEfficacite(const string& attacker, const string& defender) {
            if (mapTypes.count(attacker) && mapTypes[attacker].count(defender))
                return mapTypes[attacker][defender];
            return 1.0f;
        }
    };

class Pokemon {
    private:
        // Attributs non modifiables
        string nom; // Nom du pokémon
        vector<string> types; // Liste des types
        int maxHp; // PV max
        string atk; // Nom de l'attaque
        int dmg; // Dégâts par défaut de l'attaque

        // Stats du pokémon
        int hp; // PV actuels
        
    public:
        // Constructeur (utilisé seulement à partir du .CSV)
        Pokemon(string nom, vector<string> types, int hp, string atk, int dmg)
        : nom(nom), types(types), hp(hp), atk(atk), dmg(dmg){maxHp = hp;}
        
        // Constructeur vide
        Pokemon()
        : nom("Inconnu"), types({"Inconnu"}), hp(0), maxHp(0), atk("Aucune"), dmg(0) {}
        
        // Accesseurs
        string getNom() const {return nom;}    
        vector<string> getTypes() const {return types;}
        int getHp() const {return hp;}
        int getMaxHp() const {return maxHp;}
        string getAtk() const {return atk;}
        int getDmg() const {return dmg;}
        
        // Setters
        void setHp(int nouveauHp){hp=nouveauHp;}

        // - FONCTIONS (attaquer, KO, afficher) -

        /// @brief Fonction d'attaque d'un pokémon sur un autre
        /// @param cible Pokémon cible de l'attaque
        /// @param MEntraineur Multiplicateur de l'entraîneur
        void attaque(Pokemon& cible, double MEntraineur){
            cout << nom << " lance " << atk << endl; // Comme dans le premier jeu pokémon ;)
            double MType = 1.0; // Initialiser le multiplicateur de type

            // Récupérer le multiplicateur pour chaque type
            for (const auto& typeAttaquant : types) {
                for (const auto& typeCible : cible.getTypes()) {
                    MType *= MapTypes::getEfficacite(typeAttaquant, typeCible);
                }
            }                      

            // Calcul des dégâts réels
            double degats = dmg * MEntraineur * MType;
            // Mise à jour des HP
            int nvHp = max(0, cible.getHp() - static_cast<int>(degats));
            cible.setHp(nvHp);

            // Retour console de l'efficacité
            if(MType==0.25){ cout << "Ce n'est pas du tout efficace... " << endl; }
            else if(MType==0.5){ cout << "Ce n'est pas très efficace... " << endl; }
            else if(MType==2.0){ cout << "C'est très efficace ! " << endl; }
            else if(MType==4.0){ cout << "C'est super efficace ! " << endl; }
        }
        
        // Fonction qui retourne vrai si le pokémon est à 0 HP
        bool estKo() const { return hp <= 0; }

         /// @brief Fonction d'affichage des stats d'un pokémon 
         void afficher() const {
            // Charger les couleurs
            const string reset = "\033[0m";
            const string bold = "\033[1m";
            const string red = "\033[31m";
            const string green = "\033[32m";
            const string yellow = "\033[33m";
            const string blue = "\033[34m";
            const string cyan = "\033[36m";            
            const string magenta = "\033[35m";

            // Définir une variable rouge si pokémon KO bleu si pokémon en forme -> afficher les pokémon KO en rouge
            const string couleur = estKo() ? red : cyan; 

            // Affichage console
            cout << bold << couleur << "╔════════════════════════════════════════════╗" << reset << endl;
            int longueur = 0;
            int longueurVisible = nom.length();

            // Correction si accent ou caractères spéciaux (fait quasi cas par cas)
            if (nom.find("É") != string::npos || nom.find("é") != string::npos || nom.find("ê") != string::npos || nom.find("è") != string::npos || nom.find("-") != string::npos) {
                longueur -=1 ; // Accent = 1 colonne en moins visuellement
            }
            if (nom=="Nœunœuf"||nom=="Nidoran♀"||nom=="Nidoran♂"){longueur -=2;} // Certains types de caractères ajoutent ou enlèvent des caractères de la chaîne ASCII
            if (nom=="Mélofée"){longueur-=1;} // Cas particulier
            
            // Mise à jour de la longueur à masquer
            longueurVisible += longueur;

            // décalage
            int padding = max(0, 43 - longueurVisible); 
            cout << bold << couleur << "║ " << reset << bold << nom;
            cout << string(padding, ' ') << bold << couleur << "║" << reset << endl;

            // Affichage des types
            cout << bold << couleur << "║   Type(s): " << reset;
            ostringstream typeStream;
            longueurVisible = 0;

            // Affichage des deux types
            for (const auto& t : types) {
                typeStream << yellow << "[" << t << "] " << reset;

                // Longueur visuelle estimée : [ + nom + ] + espace
                longueur = 2 + static_cast<int>(t.length()) + 1;

                // Correction si accent ou caractères spéciaux
                if (t.find("É") != string::npos || t.find("é") != string::npos || t.find("ê") != string::npos || t.find("è") != string::npos || t.find("-") != string::npos) {
                    longueur -=1 ;
                }
                // Mise à jour de la longueur à masquer
                longueurVisible += longueur;
            }

            string typeStr = typeStream.str();
            cout << typeStr;

            // Largeur totale visuelle de la ligne = 45 (43 + "║")
            padding = max(0, 43 - 11 - longueurVisible);  // 11 = "║   Type(s): "
            cout << string(padding, ' ') << bold << couleur << "║" << reset << endl;

            // Affichage des HP (heureusement, il n'y a pas de caractères spéciaux sur des nombres)
            cout << bold << couleur << "║   HP:      " << reset 
                 << green << hp << "/" << maxHp << reset 
                 << setw(31 - to_string(hp).length() - to_string(maxHp).length()) << " " 
                 << bold << couleur << "║" << reset << endl;

                 // Affichage de l'attaque (même galère)
                 cout << bold << couleur << "║   Attaque: " << reset;
                 longueur=0;

                 // Correction si accent ou caractères spéciaux
                if (atk.find("É") != string::npos || atk.find("é") != string::npos || atk.find("ê") != string::npos || atk.find("è") != string::npos || atk.find("à")!=string::npos|| atk.find("â")!=string::npos) {
                    longueur =1 ; 
                }                
                // Cas particuliers trouvés à la main
                if (atk.find("’")!= string::npos){longueur=+2;}
                if (atk=="Écras’Face"||atk=="Coud’Krâne"){longueur+=1;}
                ostringstream atkStream;
                atkStream << magenta << atk << reset << "  |  " << red << dmg << " dégâts" << reset;
                string atkStr = atkStream.str();
                // Calcul de la longueur avec l'attaque et ses dégâts
                longueurVisible = 10 + atk.length() + 5 + to_string(dmg).length() + 7;
                cout << atkStr;
                cout << string(42 - longueurVisible +longueur, ' ') << bold << couleur << "║" << reset << endl;
            cout << bold << couleur << "╚════════════════════════════════════════════╝" << reset << endl;
        }

        /// @brief Fonction d'affichage de la barre de vie (pour les combats)
        void AfficherBarreVie() const {
            // Chargement des couleurs
            const string reset = "\033[0m";
            const string red = "\033[31m";
            const string green = "\033[32m";
            const string yellow = "\033[33m";
            const string bold = "\033[1m";
            

            // La longueur est fixe, on calcule donc le rapport HP/HP max
            double ratio = static_cast<double>(hp) / maxHp;
            int longBarre = 30;
            int rempli = static_cast<int>(ratio * longBarre);
            int vide = longBarre - rempli;
            

            // La couleur change en fonction du nombre de HP
            string color;
            if (ratio > 0.5) color = green;
            else if (ratio > 0.2) color = yellow;
            else color = red;
            
            // Sortie console = <nom> | PV: [====================] XX/YY
            cout << bold << nom << " | PV: [";
            cout << color;
        
            for (int i = 0; i < rempli; ++i)
                cout << "=";
        
            cout << reset;
        
            for (int i = 0; i < vide; ++i)
                cout << " ";
        
            cout << bold << "] " << hp << "/" << maxHp << reset << endl;
        }
    };    

/// @brief Fonction pour charger les Pokémon du CSV dans un dictionnaire
/// @param nomFichier 
/// @return Dictionnaire de Pokémon
vector<Pokemon> lirePokemonCSV(string nomFichier){
    vector<Pokemon> pokemons;
    ifstream fichier(nomFichier);
    if (!fichier) {
        cerr << "Erreur: Impossible d'ouvrir le fichier " << nomFichier << endl;
        return pokemons;
    }
    string ligne;
    getline(fichier, ligne); 
    while (getline(fichier, ligne)) {
        stringstream ss(ligne);
        string nom, type1, type2, hpStr, atk, dmgStr;
                getline(ss, nom, ',');
        getline(ss, type1, ',');
        getline(ss, type2, ',');
        getline(ss, hpStr, ',');
        getline(ss, atk, ',');
        getline(ss, dmgStr, ',');
        try {
            int hp = stoi(hpStr);
            int dmg = stoi(dmgStr);
            vector<string> types;
            // Ajouter les types
            if (!type1.empty()) types.push_back(type1);
            if (!type2.empty()) types.push_back(type2);
            // Créer et ajouter le Pokémon
            pokemons.emplace_back(nom, types, hp, atk, dmg);
        } catch (const exception& e) {
            cerr << "Erreur de lecture du fichier : " << e.what() << endl;            
        }
    }
    cout << pokemons.size() << " Pokémon chargés avec succès !" << endl;
    return pokemons;
}
