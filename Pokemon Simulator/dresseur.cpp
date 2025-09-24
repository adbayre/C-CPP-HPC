#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <algorithm>
#include "pokemon.cpp"
#include <fstream>
#include <sstream>
using namespace std;

class Dresseur {

    private :
        // Attributs non modifiables
        string nom; // Nom du dresseur
        vector<Pokemon> equipe; // Liste des pokémon
        string role; // Rôle du dresseur 

    public :
    // Constructeur 
    Dresseur(string n, vector<Pokemon> e, string r): nom(n), equipe(e), role(r){}
    
    // Accesseurs
    string getNom() const {return nom;}
    string getRole() const{return role;}
    vector<Pokemon>& getEquipe() {return equipe;}
    
    // Setters
    void setNom(string nvNom){nom = nvNom;}    
    void setRole(string nvRole){role = nvRole;}
    void setEquipe(vector<Pokemon> nvEquipe) {equipe = nvEquipe;}


    // - FONCTIONS -
    
    /// @brief Fonction pour afficher l'équipe de pokémon
    void afficherEquipe() const{
        // Afficher chaque pokémon
        for (const auto& pokemon : equipe) {
            pokemon.afficher();
        }

        // Chargement des couleurs
        const string reset = "\033[0m";
        const string bold = "\033[1m";
        const string cyan = "\033[36m";
        const string yellow = "\033[33m";        
        
        // Associer le nom du dresseur à l'équipe
        cout << bold << cyan;
        cout << "   ⬆ ⬆ ⬆     Equipe de " << bold << reset << bold << nom << "  "<< cyan <<"    ⬆ ⬆ ⬆    " << endl;
        cout << reset << endl;
    }

    // Destructeur
    virtual ~Dresseur() {}
};

class Champion : public Dresseur {
    private:
        // Attributs fixes propres au Champion
        string badge;
        string gym;
    public:
        // Constructeur avec le rôle
        Champion(string nom, vector<Pokemon> equipe, string badge, string gym): Dresseur(nom, equipe, "Champion"), badge(badge), gym(gym) {}
        
        // Destructeur
        ~Champion(){};
        
        // Accesseurs
        string getBadge() const {return badge;}
        string getArene() const {return gym;}
};

class Maitre : public Dresseur {
    public :
    // Constructeur avec le rôle
    Maitre(string nom, vector<Pokemon> equipe): Dresseur(nom, equipe, "Maître") {}

    // Destructeur
    ~Maitre(){};        
};

class Joueur : public Dresseur {
    private:
        // Attributs statistiques
        int Victoires;
        int Defaites;
        vector<string> BadgesRemportes;
    
    public:

        // Constructeur vide pour initialiser
        Joueur() : Dresseur("", {}, "Joueur"), Victoires(0), Defaites(0), BadgesRemportes() {}

        // Constructeur personnalisé
        Joueur(string nom, vector<Pokemon> equipe) : Dresseur(nom, equipe, "Joueur"), Victoires(0), Defaites(0), BadgesRemportes() {}

        // Accesseurs et setters

        // Victoires
        int getVictoires() const { return Victoires; }
        void setVictoire() { this->Victoires += 1; }
    
        // Défaites
        int getDefaites() const { return Defaites; }
        void setDefaite() { this->Defaites += 1; }

        // Liste des badges
        vector<string>& getBadgesListe() { return BadgesRemportes; }

        // - FONCTIONS -

        /// @brief Fonction pour afficher esthétiquement les badges du joueur
        void AfficherBadges() const {
            
            // Tableau de 8 emplacements
            const int totalBadges = 8;
            vector<string> badgesAffiches(totalBadges, "    "); 
        
            // Remplir les emplacements avec les badges obtenus
            for (size_t i = 0; i < BadgesRemportes.size() && i < totalBadges; ++i) {
                badgesAffiches[i] = BadgesRemportes[i];
            }
        
            // Afficher les badges
            for (const string& badge : badgesAffiches) {
                cout << "| " << badge << " ";
            }
            cout << "|" << endl;
        }        
    
        /// @brief Fonction pour afficher les statistiques du joueur
        void AfficherStatistiques() const {

            // Sortie console
            cout << "Statistiques du joueur " << getNom() << " :" << endl;
            cout << "Combats gagnés : " << Victoires << endl;
            cout << "Combats perdus : " << Defaites << endl;
            cout << "Badges remportés : ";
            AfficherBadges();
        }
    };
/// @brief Fonction pour lire un CSV de champions de gym
/// @param fichierCSV Le fichier source qui doit respecter un format fixe
/// @param pokemons pokédex
/// @param badgesDisponibles Les badges 
/// @return La liste des objets champion
vector<Champion*> lireChampionsEtBadges(const string& fichierCSV, const vector<Pokemon>& pokemons, vector<string>& badgesDisponibles){
    // Initialiser la sortie
    vector<Champion*> champions;
    ifstream fichier(fichierCSV);
    string ligne;
   
    // Ignorer l'en-tête
    getline(fichier, ligne);

    while (getline(fichier, ligne)) {
        stringstream ss(ligne);
        string nom, arene, badge;
    string p1, p2, p3, p4, p5, p6;

        // Séparation des colonnes du fichier
        getline(ss, nom, ',');
        getline(ss, arene, ',');
        getline(ss, badge, ',');
        getline(ss, p1, ',');
        getline(ss, p2, ',');
        getline(ss, p3, ',');
        getline(ss, p4, ',');
        getline(ss, p5, ',');
        getline(ss, p6, ',');
    
        // Enregistrer le badge dans la liste globale du jeu
        if (!badge.empty()) {
            badgesDisponibles.push_back(badge);
        }
    
        // Construire l'équipe de Pokémon
        vector<Pokemon> equipe;
        for (const auto& poke : pokemons) {
            if (poke.getNom() == p1) equipe.push_back(poke);
            if (poke.getNom() == p2) equipe.push_back(poke);
            if (poke.getNom() == p3) equipe.push_back(poke);
            if (poke.getNom() == p4) equipe.push_back(poke);
            if (poke.getNom() == p5) equipe.push_back(poke);
            if (poke.getNom() == p6) equipe.push_back(poke);
        }
    
        // Créer l'objet champion
        champions.push_back(new Champion(nom, equipe, arene, badge));
    }
    
    return champions;
}

/// @brief Fonction pour lire un CSV de maîtres de ligue
/// @param fichierCSV Le fichier source qui doit respecter un format fixe
/// @param pokemons pokédex
/// @return La liste des objets maître
vector<Maitre*> lireMaitres(const string& fichierCSV, const vector<Pokemon>& pokemons) {
    // Initialiser la sortie
    vector<Maitre*> maitres;
    ifstream fichier(fichierCSV);
    string ligne;

    // Ignorer l'en-tête
    getline(fichier, ligne);

    while (getline(fichier, ligne)) {
        stringstream ss(ligne);
        string nom;
        string p1, p2, p3, p4, p5, p6;

        // Séparation des colonnes du fichier
        getline(ss, nom, ',');
        getline(ss, p1, ',');
        getline(ss, p2, ',');
        getline(ss, p3, ',');
        getline(ss, p4, ',');
        getline(ss, p5, ',');
        getline(ss, p6, ',');

        // Construire l'équipe de Pokémon
        vector<Pokemon> equipe;
        for (const auto& poke : pokemons) {
            if (poke.getNom() == p1) equipe.push_back(poke);
            if (poke.getNom() == p2) equipe.push_back(poke);
            if (poke.getNom() == p3) equipe.push_back(poke);
            if (poke.getNom() == p4) equipe.push_back(poke);
            if (poke.getNom() == p5) equipe.push_back(poke);
            if (poke.getNom() == p6) equipe.push_back(poke);
        }
        // Créer l'objet maître
        maitres.push_back(new Maitre(nom, equipe));
    }
    return maitres;
}

/// @brief Fonction pour lire un CSV de joueurs
/// @param fichierCSV Le fichier source qui doit respecter un format fixe
/// @param pokemons pokédex
/// @return La liste des objets joueur
vector<Joueur*> lireJoueurs(const string& fichierCSV, const vector<Pokemon>& pokemons) {
    // Initialiser la sortie
    vector<Joueur*> joueurs;
    ifstream fichier(fichierCSV);
    string ligne;

    // Ignorer l'en-tête
    getline(fichier, ligne); 

    while (getline(fichier, ligne)) {
        stringstream ss(ligne);
        string nom;
        string p1, p2, p3, p4, p5, p6;

        // Séparation des colonnes du fichier
        getline(ss, nom, ',');
        getline(ss, p1, ',');
        getline(ss, p2, ',');
        getline(ss, p3, ',');
        getline(ss, p4, ',');
        getline(ss, p5, ',');
        getline(ss, p6, ',');

        vector<Pokemon> equipe;
        for (const auto& poke : pokemons) {
            if (poke.getNom() == p1) equipe.push_back(poke);
            if (poke.getNom() == p2) equipe.push_back(poke);
            if (poke.getNom() == p3) equipe.push_back(poke);
            if (poke.getNom() == p4) equipe.push_back(poke);
            if (poke.getNom() == p5) equipe.push_back(poke);
            if (poke.getNom() == p6) equipe.push_back(poke);
        }
        // Créer l'objet joueur
        joueurs.push_back(new Joueur(nom, equipe));
    }

    return joueurs;
}
