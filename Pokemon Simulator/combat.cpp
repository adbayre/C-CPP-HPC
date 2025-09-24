#include <iostream>
#include <cstdlib>
#include "dresseur.cpp"
using namespace std;

class Combat {
    private:
    
    // Références aux deux joueurs
    Joueur& joueur;
    Dresseur& ennemi;

    // Variables du combat
    int nbTour;
    double m;

    public:
    
    // Constructeur 
    Combat(Joueur& joueur, Dresseur& ennemi) : joueur(joueur), ennemi(ennemi){
        // Initialisation des paramètres de combat
        nbTour=0;
        // Définition du multiplicateur
        if(ennemi.getRole()=="Maître"){
            m=1.25;
        }
        else {
            m=1.0;
        }
    }

    // - FONCTIONS -
    
    /// @brief Pour avoir un combat plus joli
    void consoleClear() {
        for (int i = 0; i < 100; ++i) cout << endl;
    }

    /// @brief Déroulement d'un tour (attaque joueur puis attaque ennemi)
    /// @param indexTour Numéro du tour
    /// @param indexEnnemi Index du pokémon joué par l'ennemi
    /// @param indexJoueur Index du pokémon joué par le joueur
    void tour(int indexTour, int& indexEnnemi, int& indexJoueur){
        
        // Skip les Pokémon KO du joueur avant le tour
        while (indexJoueur < joueur.getEquipe().size() && joueur.getEquipe()[indexJoueur].estKo()) {
            /* Si on est pas au premier tour, on part du principe que le 
            dresseur essaie d'appeler le pokémon suivant, qui peut être KO*/
            if (indexTour > 1)
                cout << joueur.getEquipe()[indexJoueur].getNom() << " est KO !" << endl;
            indexJoueur++;

            // L'index est valide, on envoie le pokémon
            if (indexJoueur < joueur.getEquipe().size()) {
                cout << "Vous envoyez " << joueur.getEquipe()[indexJoueur].getNom() << " !" << endl;
                //if(joueur.getEquipe()[indexJoueur].estKo())
                 //   cout << joueur.getEquipe()[indexJoueur].getNom() << " est KO..." << endl;
                cout << "\nAppuyez sur Entrée pour continuer..." << endl;
                cin.get();
                // On nettoie la console après chaque affichage
                consoleClear();
            } 
            // Index non valide, l'équipe est vide donc le tour est fini
            else {
                cout << "Vous n'avez plus de Pokémon !" << endl;
                return;
            }
        }

        // Affichage des pokémon en jeu
        ennemi.getEquipe()[indexEnnemi].AfficherBarreVie();
        joueur.getEquipe()[indexJoueur].AfficherBarreVie();

        // Nettoyage de l'entrée précédente
        if(indexTour==1){cin.ignore(1, '\n');}
        cin.get();
        consoleClear();

        // Attaque joueur
        cout << "Vous attaquez avec " << joueur.getEquipe()[indexJoueur].getNom() << " !" << endl;
        joueur.getEquipe()[indexJoueur].attaque(ennemi.getEquipe()[indexEnnemi], m);
        ennemi.getEquipe()[indexEnnemi].AfficherBarreVie();
        joueur.getEquipe()[indexJoueur].AfficherBarreVie();
        cin.get();
        consoleClear();

        // Cas ou l'ennemi est KO
        if (ennemi.getEquipe()[indexEnnemi].estKo()){
            cout << "Le " << ennemi.getEquipe()[indexEnnemi].getNom() << " ennemi est KO !" << endl;
            indexEnnemi++;

            // L'index est valide
            if(indexEnnemi < ennemi.getEquipe().size()){
                // Pokémon vide
                if(ennemi.getEquipe()[indexEnnemi].getNom()==""){
                    cout << ennemi.getRole() << " " << ennemi.getNom() << " n'a plus de Pokémon !" << endl;
                    return;
                }
                // Si le pokémon n'est pas vide, on dit que l'ennemi envoie le pokémon suivant
                cout << ennemi.getRole() << " " << ennemi.getNom() << " envoie " << ennemi.getEquipe()[indexEnnemi].getNom() << " !" << endl;                
            }
            // Index invalide, l'équipe est KO
            else{
                cout << ennemi.getRole() << " " << ennemi.getNom() << " n'a plus de Pokémon !" << endl;
                return;
            }
            cout << "\nAppuyez sur Entrée pour continuer..." << endl;
            cin.get();
            consoleClear();
        }      

        // Attaque ennemi
        cout << ennemi.getRole() << " " << ennemi.getNom() << " attaque avec " << ennemi.getEquipe()[indexEnnemi].getNom() << " !" << endl;
        ennemi.getEquipe()[indexEnnemi].attaque(joueur.getEquipe()[indexJoueur], m);
        ennemi.getEquipe()[indexEnnemi].AfficherBarreVie();
        joueur.getEquipe()[indexJoueur].AfficherBarreVie();
        cin.get();
        consoleClear();

        // Si le pokémon joueur a été mis KO par l'attaque
        if(joueur.getEquipe()[indexJoueur].estKo()){
            cout << joueur.getEquipe()[indexJoueur].getNom() << " est KO !" << endl;
            indexJoueur++;

            // L'index est valide
            if(indexJoueur<joueur.getEquipe().size()){
                cout << "Vous envoyez " << joueur.getEquipe()[indexJoueur].getNom() << " !" << endl;
            }
            // Sinon, l'équipe est KO
            else {
                cout << "Vous n'avez plus de Pokémon !" << endl;
                return;
            }
            cout << "\nAppuyez sur Entrée pour continuer..." << endl;
            cin.get();
            consoleClear();
        }
        
    }

    /// @brief Ensemble de tour qui continue jusqu'à qu'une des équipes soit KO
    void partie(){        
        // Initialisation
        int indexJoueur=0;
        int indexEnnemi=0;

        // Chargement des couleurs
        const string bold = "\033[1m";
        const string reset = "\033[0m";
        const string yellow = "\033[33m";
        const string orange = "\033[38;5;208m";

        // Orange si l'ennemi est maître de la ligue, jaune sinon
        const string couleur = (ennemi.getRole()=="Maitre") ? orange : yellow; 

        // Affichage des participants de la partie
        cout << bold << couleur << "~ Entraîneur " << joueur.getNom() << " VS " << ennemi.getRole() << " " << ennemi.getNom() << " ~" << reset << endl;
        int indexTour = 0;
        // Tant qu'aucune équipe n'est KO, on lance des tours
        while(indexJoueur < joueur.getEquipe().size() && indexEnnemi < ennemi.getEquipe().size()) {
            indexTour++;
            tour(indexTour, indexEnnemi, indexJoueur);
        }
        
        // Cas ou le joueur a perdu
        if(indexJoueur >= joueur.getEquipe().size()){
            cout << "Vous avez été vaincu par " << ennemi.getRole() << " " << ennemi.getNom() << "..." << endl;
            for (auto& pkm : ennemi.getEquipe()) {         
                pkm.setHp(pkm.getMaxHp());                
            }
            joueur.setDefaite();
        }

        // Autre cas le joueur a gagné
        else{
            cout << "Vous avez vaincu " << ennemi.getRole() << " " << ennemi.getNom() << " !" << endl;
            joueur.setVictoire();

            // Attribution du badge si l'ennemi est un champion de gym
            if (ennemi.getRole() == "Champion") {
                Champion* champion = dynamic_cast<Champion*>(&ennemi);
                if (champion != nullptr) {
                    cout << "Vous obtenez le badge " << champion->getBadge() << " !" << endl;
                    joueur.getBadgesListe().push_back(champion->getBadge());
                } else {
                    // Exception
                    cerr << "Le cast de Dresseur* vers Champion* a échoué." << endl;
                }
            }            
        }        
    }
};