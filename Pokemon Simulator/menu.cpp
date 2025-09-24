#include <iostream>
#include <limits>
#include <algorithm>
#include <locale>
#include <iterator>
#include <set>
#include "combat.cpp"
using namespace std;
// - MENU.CPP : FICHIER A COMPILER -
// JOUER AVEC JOUEUR MAJD POUR BATTRE LA LIGUE ET TESTER TOUTES LES FONCTIONNALITES 

// Efface la console (en simulant par des sauts de 100 lignes)
void consoleClear() {
    for (int i = 0; i < 100; ++i) cout << endl;
}

/// @brief Fonction pour afficher le pokédex en liste de Pokémons
/// @param pokedex 
void afficherPokedex(vector<Pokemon> pokedex){

    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string cyan = "\033[36m";
    const string yellow = "\033[33m";
    // Affichage de tous les pokémon
    for(Pokemon pokemon : pokedex){
        pokemon.afficher();

    }
    // Sortie console
    cout << bold << cyan;
    cout << "╔═════════════════════════════════════════════════════════╗" << endl;
    cout << "║   "<< reset <<"    ⬆ ⬆ ⬆      📘  POKÉDEX NATIONAL       ⬆ ⬆ ⬆ "<< cyan << "      ║" << endl;
    cout << "╠═════════════════════════════════════════════════════════╣" << endl;
    cout << "║ "<< yellow <<"Voici la liste complète des Pokémon connus à ce jour !" << cyan <<"  ║" << endl;
    cout << "╚═════════════════════════════════════════════════════════╝" << reset << endl << endl;
}

/// @brief Demande une saisie utilisateur avec message personnalisé
/// @param message 
/// @return La saisie utilisateur
string entree(const string& message) {
    cout << message;
    string input;
    getline(cin >> ws, input);
    return input;
}

/// @brief Fonction pour standardiser les mots (noms de pokémon, de type etc)
/// @param str 
/// @return L'entrée standardisée
string enleverAccents(const string &str) {
    string sortie = str;

    // Utilisation de chaînes pour gérer les caractères multioctets
    const vector<pair<string, string>> accents = {
        {"é", "e"}, {"è", "e"}, {"ê", "e"}, {"ë", "e"},
        {"à", "a"}, {"â", "a"}, {"ä", "a"}, {"É", "e"},
        {"î", "i"}, {"ï", "i"},
        {"ô", "o"}, {"ö", "o"},
        {"ù", "u"}, {"û", "u"}, {"ü", "u"},
        {"ç", "c"}
    };

    // On remplace dans le string le caractère accentué
    for (const auto& [accent, remplacement] : accents) {
        size_t pos = 0;
        while ((pos = sortie.find(accent, pos)) != string::npos) {
            sortie.replace(pos, accent.length(), remplacement);
            pos += remplacement.length();
        }
    }

    transform(sortie.begin(), sortie.end(), sortie.begin(), ::tolower);

    return sortie;
}

/// @brief Méthode pour rechercher un pokémon du pokédex
/// @param recherche
/// @param pokedex 
/// @return Le pokémon trouvé par la méthode
Pokemon rechercherPokemon(const string &recherche, const vector<Pokemon> &pokedex) {
    string rechercheCorr = enleverAccents(recherche);
    vector<const Pokemon*> correspondances;

    // Cherche tous les Pokémon dont le nom sans accent contient le texte recherché
    for (const auto &pokemon : pokedex) {
        string nomSansAccent = enleverAccents(pokemon.getNom());

        if (nomSansAccent.find(recherche) != string::npos) {
            correspondances.push_back(&pokemon);
        }
    }

    // Aucune correspondance trouvée, on relance la recherche
    if (correspondances.empty()) {
        cout << "❌ Aucun Pokémon trouvé correspondant à '" << recherche << "'. Veuillez réessayer." << endl;
        return rechercherPokemon(entree("Nom du pokémon : "), pokedex); // Fonction récursive jusqu'à succès
    }

    // Si une seule correspondance parfaite -> retour direct
    for (const Pokemon* p : correspondances) {
        if (enleverAccents(p->getNom()) == rechercheCorr || correspondances.size()==1) {
            cout << "✅ Correspondance trouvée : " << p->getNom() << endl;
            p->afficher();
            return *p;
        }
    }

    // Plusieurs correspondances, on va demander à l'utilisateur de choisir
    cout << "🔍 Plusieurs correspondances trouvées : " << endl;
    for (size_t i = 0; i < correspondances.size(); ++i) {
        correspondances[i]->afficher();
    }
    for (size_t i = 0; i < correspondances.size(); ++i) {
        cout << i+1 << ". " << correspondances[i]->getNom() << endl;
    }

    size_t choix = 0;
    // Choisir un index
    while (true) {
        cout << "👉 Entrez le numéro du Pokémon que vous voulez choisir : ";
        cin >> choix;
        if (choix >= 1 && choix <= correspondances.size()) {
            break;
        }
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "❌ Choix invalide. Essayez encore." << endl;
    }
    return *(correspondances[choix - 1]);
}

/// @brief Méthode qui correspond au centre pokémon
/// @param equipe 
void soinEquipe(vector<Pokemon>& equipe)
{
    if (equipe.empty()) {
        cout << "Votre équipe est vide. Aucun soin nécessaire." << endl;
        return;
    }
    // Textes copiés de let's play de pokémon rouge
    cout << "Bonjour! Bienvenue au Centre Pokémon, où nous \nsoignons les Pokémons." << endl;
    cout << "Voulez-vous que nous nous occupions de votre équipe ? (O/N) : ";
    char reponse;
    cin >> reponse;
    reponse = toupper(reponse);

    if (reponse != 'O') {
        cout << "Très bien, à bientôt au Centre Pokémon !" << endl;
        return;
    }
    // Interface de soin
    cout << "Très bien ! Confiez-moi votre équipe un instant." << endl;
    cin.get();
    for (auto& pkm : equipe) {        
        if (pkm.getHp() < pkm.getMaxHp()) {
            cout << pkm.getNom() << " a retrouvé toute son énergie !" << endl;
            pkm.setHp(pkm.getMaxHp());
        } else {
            cout << pkm.getNom() << " était déjà en pleine forme !" << endl;
        }
    }
    cin.get();
    cout << "Merci d’avoir attendu. À bientôt !" << endl;
}

/// @brief Fonction d'interaction
/// @param equipe 
void interagirEquipe(vector<Pokemon>& equipe) {
    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string cyan = "\033[36m";
    const string green = "\033[32m";
    const string red = "\033[31m";

    if (equipe.empty()) {
        cout << red << "Votre équipe est vide !" << reset << endl;
        return;
    }

    // Affichage console
    cout << bold << cyan << "Voulez-vous interagir avec un de vos Pokémon ?" << reset << endl;
    cout << green << "Choisissez un numéro de Pokémon :" << reset << endl;

    // Affichage des pokémon
    for (size_t i = 0; i < equipe.size(); ++i) {
        cout << i + 1 << ". " << equipe[i].getNom() << endl;
    }

    cout << "\nEntrez votre choix (1 à " << equipe.size() << ", 0 pour annuler) : ";
    int choix;
    cin >> choix;

    if (choix == 0) {
        cout << red << "Interaction annulée." << reset << endl;
        return;
    }

    // Sortie d'un cri aléatoire parmi la liste
    if (choix >= 1 && choix <= equipe.size()) {
        vector<string> cris = {"Pip", "Grrr", "Reeee", "Hop", "Zoup"};
        string cri = cris[rand() % cris.size()];
    
        cout << green << cri << " ! Je suis " << equipe[choix - 1].getNom() << " !" << reset << endl;
    } else {
        cout << red << "Choix invalide." << reset << endl;
    }
}

/// @brief Fonction du menu qui permet de réorganiser une équipe, ou interagir
/// @param joueur 
void gererEquipe(Joueur* joueur) {
    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string green = "\033[32m";
    const string red = "\033[31m";

    joueur->afficherEquipe();

    if (joueur->getEquipe().empty()) {
        cout << red << "Vous n'avez aucun Pokémon dans votre équipe." << reset << endl;
        return;
    }

    // On prompt l'utilisateur
    cout << bold << "Que souhaitez-vous faire ?" << reset << endl;
    cout << "1. 🔀 Réordonner l’équipe" << endl;
    cout << "2. 🧸 Interagir avec un Pokémon" << endl;
    cout << "0. Retour" << endl;
    cout << green << "Votre choix : " << reset;
    int choix;
    cin >> choix;
    consoleClear();

    // Fonction pour réordonner l'équipe
    switch (choix) {
        case 1: {
            vector<Pokemon>& equipe = joueur->getEquipe();
            cout << "\nVoici votre équipe :" << endl;
            const string reset = "\033[0m";
            const string green = "\033[32m";
            
            // Affichage de l'équipe
            for (size_t i = 0; i < equipe.size(); ++i) {
                cout << green << i + 1 << ". " << equipe[i].getNom() << reset << endl;
            }
            cout << "Entrez les indices (1 à " << equipe.size() << ") de deux Pokémon à échanger :\n";
            int i, j;
            cin >> i >> j;
            // Si on valide les index, l'équipe est réorganisée
            if (i >= 1 && i <= equipe.size() && j >= 1 && j <= equipe.size()) {
                swap(equipe[i - 1], equipe[j - 1]);
                cout << green << "L’équipe a été réorganisée." << reset << endl;
                // On réaffiche l'équipe sinon c'est pas très clair
                joueur->afficherEquipe();
            } else {
                cout << red << "Indices invalides." << reset << endl;
            }
            break;
        }
        case 2:
            // On lance la fonction d'interaction
            interagirEquipe(joueur->getEquipe());
            break;
        case 0:
            break;
        default:
            cout << red << "Choix invalide." << reset << endl;
    }
}

/// @brief Affichage des champions de gym à combattre ou interagir
/// @param joueur 
/// @param champions 
void afficheChampionsDisponibles(Joueur* joueur, vector<Champion*>& champions) {
    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string cyan = "\033[36m";
    const string yellow = "\033[33m";
    const string red = "\033[31m";
    const string green = "\033[32m";

    while (true) {
        consoleClear();
        vector<string> badgesObtenus = joueur->getBadgesListe();

        // Affichage 
        cout << bold << cyan << "\n=== Liste des Champions de Gym ===" << reset << endl;
        for (size_t i = 0; i < champions.size(); ++i) {
            const string& nom = champions[i]->getNom();
            const string& arene = champions[i]->getArene();
            const string& badge = champions[i]->getBadge();
            // On prend note des champions déjà vaincus
            bool dejaVaincu = find(badgesObtenus.begin(), badgesObtenus.end(), badge) != badgesObtenus.end();

            cout << yellow << (i + 1) << ". " << nom << " (" << arene << ")" << (dejaVaincu ? green + string(" [Déjà vaincu]") : "") << reset << endl;
        }

        cout << yellow << "0. " << reset << "Retour au menu principal" << endl;
        cout << "\nFaites votre choix (0 pour retour) : ";
        int choix;
        cin >> choix;
        if (choix == 0) return;
        if (cin.fail() || choix < 1 || choix > (int)champions.size()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << red << "Choix invalide. Réessayez." << reset << endl;
            cin.get();
            continue;
        }
        // On enregistre un pointeur vers le champion choisi
        Champion* championChoisi = champions[choix - 1];
        string badge = championChoisi->getBadge();
        bool dejaVaincu = find(badgesObtenus.begin(), badgesObtenus.end(), badge) != badgesObtenus.end();

        // Si on prompt le champion déjà vaincu, on a une interaction personnalisée
        if (dejaVaincu) {
            consoleClear();
            cout << bold << cyan << championChoisi->getNom() << " : " << reset
                 << "Bravo dresseur ! Tu as déjà remporté le badge " << badge << "." << endl;
            cout << "Tu peux venir discuter avec moi quand tu veux !" << endl;
            cin.ignore();
            cin.get();
        // Sinon, on lance un combat
        } else {
            Combat combat(*joueur, *championChoisi);
            consoleClear();
            combat.partie();
            // Si à l'issue du combat le joueur a perdu, on l'envoie au centre pokémon
            bool tousKo = true;
            for (const Pokemon& p : joueur->getEquipe()) {
                if (!p.estKo()) {
                    tousKo = false;
                    break;
                }
            }

            if (tousKo) {
                cout << red << "\nTous vos Pokémon sont K.O. ! Direction le Centre Pokémon..." << reset << endl;
                soinEquipe(joueur->getEquipe());
                cin.get();
            }
        }
    }
}

/// @brief Même fonction que pour les champions pour les maîtres de ligue
/// @param joueur 
/// @param maitres 
void afficheMaitresDisponibles(Joueur* joueur, vector<Maitre*>& maitres) {
    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string cyan = "\033[36m";
    const string orange = "\033[38;5;208m";
    const string red = "\033[31m";
    // Affichage console
    cout << bold << cyan << "\nListe des Maîtres Pokémon :" << reset << endl;
    for (int i = 0; i < maitres.size(); ++i) {
        cout << orange << i << ". "
             << maitres[i]->getNom() << " (" << maitres[i]->getEquipe()[0].getTypes()[0] << ")" << reset << endl;
    }
    cout << "Appuyez sur Entrée pour continuer..." << endl;
    cin.ignore(1, '\n');
    cin.get();
    int choix;
    // Choix aléatoire du maître de ligue
    srand(static_cast<unsigned int>(time(0)));
    choix = rand() % maitres.size();
    if (choix >= 0 && choix < maitres.size()) {
        Combat c(*joueur, *maitres[choix]);
        consoleClear();
        c.partie();

        // Soigner si tous les Pokémon sont K.O.
        bool tousKo = true;
        for (const Pokemon& p : joueur->getEquipe()) {
            if (!p.estKo()) {
                tousKo = false;
                break;
            }
        }

        if (tousKo) {
            cout << red << "\nTous vos Pokémon sont K.O. ! Direction le Centre Pokémon..." << reset << endl;
            soinEquipe(joueur->getEquipe());
        } else {
            delete maitres[choix]; 
            maitres.erase(maitres.begin() + choix);
        }
    } else {
        cout << red << "Choix invalide." << reset << endl;
    }
}

/// @brief Affichage des crédits si l'on a vaincu la ligue
/// @param joueur 
void credits(Joueur* joueur){
    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string yellow = "\033[33m";
    const string cyan = "\033[36m";
    // Panthéon des pokémon
    for(int i = 0; i<joueur->getEquipe().size(); i++){
        joueur->getEquipe()[i].afficher();        
        cout << bold << cyan << "╔════════════════════════════════════════════╗" << reset << endl;
        cout << bold << cyan << "║" << yellow << bold << "                  CHAMPION                  " << bold << cyan << "║" << endl;
        cout << bold << cyan << "╚════════════════════════════════════════════╝" << reset << endl;
        cin.get();
        
    }
    // Affichage des statistiques du joueur en fin de jeu
    joueur->AfficherStatistiques();
    cout << bold << "\n~ Jeu créé par Adrien Bayre et Tom Bernhard ~" << reset << endl;
    cin.get();
    exit(0);
}

/// @brief Fonction du prémenu pour sélectionner parmi les joueurs du CSV
/// @param joueurs 
/// @return Le joueur choisi
Joueur* choisirDresseurExistant(vector<Joueur*> joueurs) {
    if (joueurs.empty()) {
        cout << "Aucun dresseur n'est disponible pour le moment." << endl;
        return nullptr;
    }

    cout << "Liste des dresseurs disponibles :" << endl;
    for (size_t i = 0; i < joueurs.size(); ++i) {
        cout << i + 1 << ". " << joueurs[i]->getNom() << endl;
    }

    int choix = 0;
    while (true) {
        cout << "Entrez le numéro du dresseur à sélectionner : ";
        cin >> choix;

        if (cin.fail() || choix < 1 || choix > (int)joueurs.size()) {
            cout << "Entrée invalide. Veuillez réessayer." << endl;
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
        } else {
            break;
        }
    }

    return joueurs[choix - 1];
}

/// @brief Fonction du prémenu pour créer un joueur à la main (ou avec équipe aléatoire)
/// @param pokedex 
/// @param tailleEquipe 
/// @return Le joueur avec son équipe
Joueur* creerEquipeManuelle(const vector<Pokemon> &pokedex, int tailleEquipe = 6) {

    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string cyan = "\033[36m";
    const string yellow = "\033[33m";

    // Demander le nom du joueur
    string nomJoueur;
    cout << bold << cyan << "=== Création manuelle d'équipe Pokémon ===" << reset << endl;
    cout << yellow << "Entrez le nom de votre dresseur : " << reset;
    getline(cin >> ws, nomJoueur);

    // Initialisation de la future équipe
    vector<Pokemon> equipe;

    char mode;
    cout << "Choisir entre aléatoire (A) ou manuel (autre)";
    cin >> mode;
    mode = toupper(mode);
    // Choix aléatoire
    if (mode == 'A')
{
    srand(time(0));
    set<string> typesEquipe;
    set<string> nomsUtilisés;
    while ((int)equipe.size() < tailleEquipe) {
        int index = rand() % pokedex.size();
        Pokemon p = pokedex[index];

        // Vérifie que ce Pokémon n'est pas déjà dans l’équipe
        if (nomsUtilisés.count(p.getNom()) > 0) continue;

        // Vérifie si c’est le dernier Pokémon à ajouter et si ça respecte les 3 types minimum
        set<string> typesAvecNouveau = typesEquipe;
        for (const string& type : p.getTypes()) {
            typesAvecNouveau.insert(type);
        }

        if (equipe.size() == tailleEquipe - 1 && typesAvecNouveau.size() < 3) {
            continue;
        }

        // Ajout accepté
        equipe.push_back(p);
        nomsUtilisés.insert(p.getNom());
        for (const string& type : p.getTypes()) {
            typesEquipe.insert(type);
        }

        p.afficher();
        cout << bold << cyan << p.getNom() << reset << " a été ajouté à l’équipe !" << endl;
        cout << "Pokémon dans l'équipe : " << equipe.size() << "/" << tailleEquipe << endl;
        if (equipe.size() == 1)
            cin.ignore(1, '\n');
        cin.get();
    }
}
    else {
        while ((int)equipe.size() < tailleEquipe) {
            cout << "\n" << yellow << "Tapez un nom de Pokémon, ou 'afficher' pour voir tout le Pokédex : " << reset;
            string input;
            getline(cin >> ws, input);
            consoleClear();
            if (input == "afficher") {
                for (const Pokemon &pokemon : pokedex) {
                    pokemon.afficher();
                }
                continue;
            }

            Pokemon p = rechercherPokemon(input, pokedex);
            
            // Vérifie qu'il n'est pas déjà dans l'équipe
            bool dejaDansEquipe = false;
            for (const Pokemon& pk : equipe) {
                if (pk.getNom() == p.getNom()) {
                    cout << "⚠️ Ce Pokémon est déjà dans l'équipe." << endl;
                    dejaDansEquipe = true;
                    break;
                }
            }

            if (dejaDansEquipe) continue;

            // Créer un set avec tous les types actuels de l’équipe
            set<string> typesExistants;
            for (const Pokemon& pk : equipe) {
                for (const string& type : pk.getTypes()) {
                    typesExistants.insert(type);
                }
            }

            // Ajouter temporairement les types du nouveau Pokémon
            set<string> typesAvecNouveau = typesExistants;
            for (const string& type : p.getTypes()) {
                typesAvecNouveau.insert(type);
            }

            // Vérifie s’il y aurait au moins 3 types différents avec ce nouveau Pokémon
            if (equipe.size() == tailleEquipe - 1 && typesAvecNouveau.size() < 3) {
                cout << "⚠️ L’ajout de ce Pokémon ne permet pas d’avoir au moins 3 types différents dans l’équipe." << endl;
                continue;
            }

            // Ajout final
            equipe.push_back(p);
            cout << bold << cyan << p.getNom() << reset << " a été ajouté à l’équipe !" << endl;
            cout << "Pokémon dans l'équipe : " << equipe.size() << "/" << tailleEquipe << endl;
        }
    }    

    cout << bold << cyan << "\n✅ Équipe complète !" << reset << endl;
    cin.get();
    // Crée un objet Joueur et retourne un pointeur
    Joueur* joueurPerso = new Joueur(nomJoueur, equipe);
    return joueurPerso;
}

/// @brief Pré-menu de création de joueur
/// @param joueurs 
/// @param pokedex 
/// @return Le joueur créé au terme du prémenu
Joueur* menuOuverture(vector<Joueur*> joueurs, vector<Pokemon>& pokedex) {
    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string blue = "\033[34m";
    const string green = "\033[32m";
    const string yellow = "\033[33m";
    const string red = "\033[31m";

    int choix;
    Joueur* joueur = nullptr;

    do {
        // Affichage
        consoleClear();
        cout << bold << blue;
        cout << "╔══════════════════════════════════════════════╗" << endl;
        cout << "║                🎮  LANCEMENT                 ║" << endl;
        cout << "╠══════════════════════════════════════════════╣" << endl;
        cout << "║ 1. 👤​ Utiliser un dresseur existant          ║" << endl;
        cout << "║ 2. ➕ Créer un nouveau joueur                ║" << endl;            
        cout << "║ 3. ❌ Quitter le jeu                         ║" << endl;
        cout << "╚══════════════════════════════════════════════╝" << reset << endl;

        cout << bold << green << "Votre choix : " << reset;
        cin >> choix;
        // Si le choix est mauvais
        if (cin.fail() || choix < 1 || choix > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << red << "Option invalide. Veuillez entrer un numéro entre 1 et 3." << reset << endl;
            cout << "\nAppuyez sur Entrée pour continuer..." << endl;
            cin.ignore();
            cin.get();
            continue;
        }
        // nettoyer après le int
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (choix) {
            case 1:
                // Le joueur prend un dresseur existant
                joueur = choisirDresseurExistant(joueurs);
                if (joueur != nullptr){
                    cout << bold << yellow << "Lancement du jeu avec le dresseur " << joueur->getNom() << "..." << reset << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    return joueur;
                }

                break;

            case 2:
                // Le joueur veut créer son personnage
                joueur = creerEquipeManuelle(pokedex);
                cout << bold << yellow << "Lancement du jeu avec le dresseur " << joueur->getNom() << "..." << reset << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  
                    cin.get();
                return joueur;

            case 3:
                cout << green << "À bientôt, dresseur !" << reset << endl;
                break;
        }

        cout << "\nAppuyez sur Entrée pour continuer..." << endl;
        cin.get();

    } while (choix != 3);

    return nullptr;
}

/// @brief Fonction menu principal
/// @param joueur 
/// @param pokedex 
/// @param champions 
/// @param maitres 
/// @param nbBadges 
void menu(Joueur* joueur, vector<Pokemon>& pokedex, vector<Champion*>& champions, vector<Maitre*> maitres, int nbBadges) {
    // Chargement des couleurs
    const string reset = "\033[0m";
    const string bold = "\033[1m";
    const string blue = "\033[34m";
    const string green = "\033[32m";
    const string yellow = "\033[33m";
    const string red = "\033[31m";

    int choix;

    do {
        // Affichage 
        consoleClear();
        cout << bold << blue;
        cout << "╔══════════════════════════════════════════════╗" << endl;
        cout << "║              🎮  MENU PRINCIPAL              ║" << endl;
        cout << "╠══════════════════════════════════════════════╣" << endl;
        cout << "║ 1. 📘 Afficher le Pokédex                    ║" << endl;
        cout << "║ 2. 👥 Afficher mon équipe                    ║" << endl;
        cout << "║ 3. ⚔️  Lancer un combat contre un champion    ║" << endl;
        cout << "║ 4. 📊 Afficher les statistiques du joueur    ║" << endl;

        // Afficher l’option maitre uniquement si tous les badges sont obtenus
        if (joueur->getBadgesListe().size() == nbBadges) {
            cout << "║ 5. 👑 Affronter un Maître Pokémon            ║" << endl;
            cout << "║ 6. 🏥 Aller au Centre Pokémon (soin)         ║" << endl;
            cout << "║ 7. ❌ Quitter le jeu                         ║" << endl;
        } else {
            cout << "║ 5. 🏥 Aller au Centre Pokémon (soin)         ║" << endl;
            cout << "║ 6. ❌ Quitter le jeu                         ║" << endl;
        }

        cout << "╚══════════════════════════════════════════════╝" << reset << endl;
        cout << bold << green << "Votre choix : " << reset;
        // On continue tant que le joueur ne quitte pas 
        while (!(cin >> choix) || choix < 1 || choix > (joueur->getBadgesListe().size() == nbBadges ? 7 : 6)) {
            cin.clear();  
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  
            cout << red << "Option invalide. Veuillez entrer un numéro entre 1 et " << (joueur->getBadgesListe().size() == nbBadges ? 7 : 6) << "." << reset << endl;
            cout << bold << green << "Votre choix : " << reset;
        }

        // Gestion des choix selon l’état des badges
        if (joueur->getBadgesListe().size() == nbBadges) {
            switch (choix) {
                case 1:
                    afficherPokedex(pokedex);
                    break;
                case 2:
                    gererEquipe(joueur);
                    break;
                case 3:
                    afficheChampionsDisponibles(joueur, champions);
                    break;
                case 4:
                    joueur->AfficherStatistiques();
                    break;                
                case 5:                 
                    afficheMaitresDisponibles(joueur, maitres);
                    if(maitres.size()==0){
                        credits(joueur);
                    }
                    break;                
                case 6:
                    soinEquipe(joueur->getEquipe());
                    break;
                case 7:
                    cout << green << "À bientôt, dresseur !" << reset << endl;
                    break;
                default:
                    cout << red << "Option invalide. Veuillez réessayer." << reset << endl;
            }
        } else {
            switch (choix) {
                case 1:
                    afficherPokedex(pokedex);
                    break;
                case 2:
                    gererEquipe(joueur);
                    break;
                case 3: 
                    afficheChampionsDisponibles(joueur, champions);
                    break;                  
                case 4:
                    joueur->AfficherStatistiques();
                    break;                
                case 5:
                    soinEquipe(joueur->getEquipe());
                    break;
                case 6:
                    cout << green << "À bientôt, dresseur !" << reset << endl;
                    break;
                default:
                    cout << red << "Option invalide. Veuillez réessayer." << reset << endl;
            }
        }

        cout << "\nAppuyez sur Entrée pour continuer..." << endl;
        cin.ignore(1, '\n');
        cin.get();
        consoleClear();

    } while ((joueur->getBadgesListe().size() == nbBadges && choix != 7) ||
             (joueur->getBadgesListe().size() != nbBadges && choix != 6));
}

/// @brief Main fonction
/// @return 0 si le code compile
int main(){
// - INITIALISATION -
    vector<string> badges;
    vector<Pokemon> pokemons = lirePokemonCSV("pokemon.csv");
    vector<Joueur*> joueurs = lireJoueurs("joueur.csv", pokemons);
    consoleClear();
// - MENU D'OUVERTURE -
    // Choisir Majd pour tout tester car son équipe est quasi une des seules qui bat toute la ligue
    Joueur* joueur = menuOuverture(joueurs, pokemons);
// - INITIALISATION DU JEU -
    vector<Maitre*> maitres = lireMaitres("maitres.csv", pokemons);
    vector<Champion*> champions = lireChampionsEtBadges("leaders.csv", pokemons, badges);    
    int nbBadges = champions.size();
    cout << "Jeu chargé avec succès !" << endl;
// - LANCEMENT DE LA PARTIE -
    menu(joueur, pokemons, champions, maitres, nbBadges);
    return 0;
}