

## Le Concept : "Spectrum Blaster"
Le jeu est un shoot'em up à défilement horizontal. Le but n'est pas de tout détruire, mais de survivre en se créant des passages à travers des murs de blocs colorés. Le "Merge" intervient dans la capacité du joueur à fusionner ses types de tirs pour correspondre aux couleurs complexes des obstacles.

## Gameplay Core :
- Le Vaisseau et ses Canons : Le vaisseau du joueur possède trois types de canons de base : Rouge, Bleu, et Violet.
- Le Puzzle de Couleurs :
    - L'écran défile et présente régulièrement des murs de blocs. Chaque bloc a une couleur spécifique. Ce sont les blocs qui défilent et non l'écran
    - Un bloc Rouge est détruit par un tir Rouge. Il est immunisé contre un tir Bleu ou Violet seul.
    - Un bloc Bleu est immunisé contre un tir Rouge ou Violet seul. Il faut un tir Bleu.
    - Un bloc Violet est immunisé contre un tir Rouge ou Bleu seul. Il faut un tir Violet.
- La Fusion de Tirs (Le "Merge") : C'est ici que le joueur doit résoudre le puzzle en temps réel.
    - Touche A : Mode/Tir Rouge (Absorbe les balles rouges).
    - Touche Z : Mode/Tir Bleu (Absorbe les balles bleues).
    - Touches A + Z enfoncées ensemble : FUSION ! Mode/Tir Violet (Absorbe le violet, brise les blocs violets).

Le Challenge : Le joueur doit naviguer entre les obstacles et les petits ennemis (qui se contentent de tirer des projectiles simples qu'il faut esquiver) tout en gérant mentalement les combinaisons de touches pour fusionner la bonne couleur et détruire le bloc qui bloque son chemin.

Pourquoi c'est parfait pour Raylib :
Ce concept utilise les forces de Raylib tout en restant gérable dans le temps imparti d'une Game Jam :

- Gestion des inputs : Raylib gère nativement les pressions simultanées (IsKeyDown(KEY_A) && IsKeyDown(KEY_S)), ce qui est le cœur de la mécanique de fusion.
- Logique de Fusion : La fusion de couleurs RGB est très simple à coder en C. Vous pouvez stocker la couleur cible du bloc et la couleur actuelle du tir, et vérifier leur correspondance avec un simple test conditionnel lors de la collision.
- Affichage minimaliste et percutant : Vous pouvez représenter le vaisseau, les tirs et les blocs avec des formes géométriques de base (DrawRectangleRec, DrawCircle, etc.) en utilisant des couleurs vives et un effet de flou (fade) pour les lasers fusionnés. L'esthétique "Néon Rétro" de l'image de visualisation s'obtient très facilement.

## Deadline
12/07/2026 8PM

## Planning

### 👾 Mercredi 08/07/2026: Le Moteur de Base (Gameplay brut)

* **Matin :** Initialisation de la fenêtre en $720 \times 720$ pixels. Setup du vaisseau (un triangle `DrawTriangle`) qui bouge en $X$ et $Y$ avec les flèches ou ZQSD.
* **Après-midi :** Gestion des inputs (A, Z, et A+Z). Changement de la couleur du vaisseau et de son type de tir selon les touches pressées.
* **Soirée :** Système de tir du joueur (génération de rectangles/cercles colorés qui montent).

### 🧱 Jeudi 09/07/2026 : Les Obstacles et Ennemis (Le Puzzle)

* **Matin :** Logique d'apparition des ennemis (des carrés) et des murs de blocs. Faites-les simplement descendre sur l'axe $Y$.
* **Après-midi :** Les collisions critiques (le cœur du jeu) :
* Si balle ennemie touche joueur : même couleur = absorption (score +1), couleur différente = Game Over.
* Si tir joueur touche bloc : même couleur = destruction du bloc, couleur différente = le tir disparaît sans effet.


* **Soirée :** Implémentation du Game Over et de la boucle de redémarrage automatique (`Restart`). À ce stade, votre jeu est **jouable**.

### 🎨 Vendredi 10/07/2026 : Structure et "Juice" (Rendre le jeu fun)

* **Matin :** Création d'un système de vagues très simple (ex: Vague 1 = que du Rouge/Bleu, Vague 2 = apparition des blocs Violets fusionnés).
* **Après-midi :** Interface utilisateur (Score, Combo, Écran de Game Over) avec `DrawText`.
* **Soirée (Le "Juice") :** Ajoutez des effets visuels simples mais percutants. Quand un bloc explose, générez quelques petites particules de la même couleur (`DrawCircle` avec une vitesse aléatoire qui diminue). Ajoutez un léger tremblement d'écran (*screen shake*) lors des fusions ou des destructions.

### 🚀 Samedi 11 et Dimanche 12/07/2026 : Audio, Build et Publication

* **Matin :** Intégration du son. Utilisez des générateurs de bruitages rétro gratuits comme **bfxr** ou **sfxr** (générez un son de tir, un son d'explosion, et un son d'absorption). Raylib gère l'audio très facilement avec `LoadSound()` et `PlaySound()`.
* **Après-midi :** Chasse aux bugs, équilibrage de la vitesse du jeu et export du build (assurez-vous que l'exécutable tourne bien en mode standalone).
* **Fin d'après-midi :** Création de la page sur itch.io, rédaction d'une description simple avec les touches du clavier, et **upload avant la deadline** !

---

### 💡 3 Règles d'or pour ne pas déraper :

1. **Pas de sprites (images) :** Utilisez uniquement les fonctions de dessin de formes géométriques de Raylib (`DrawRectangle`, `DrawCircle`, `DrawTriangle`). C'est propre, néon, et ça prend 0 seconde à charger.
2. **Pas de scrolling infini complexe :** Faites simplement descendre vos vagues d'ennemis/blocs en modifiant leur position $Y$. Quand un bloc dépasse $Y > 720$, vous le détruisez de la mémoire.
3. **Soignez l'impact :** Un jeu de jam avec des graphismes basiques mais de super sensations de destruction (particules + flashs de couleur + bons bruitages) obtiendra toujours une meilleure note qu'un jeu complexe mais mou.

Par quel morceau du code comptez-vous attaquer ce soir ou demain matin ?


