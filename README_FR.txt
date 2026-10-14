MOTOCROSS ENGINE — Akai Force Gen 1
====================================

Ce dossier contient une première version du projet et un workflow de compilation distante.
Ce n'est PAS encore le ZIP installable : il faut que la compilation distante réussisse.

Ce que fait l'instrument :
- son moteur synthétique continu (pas un enregistrement réaliste) ;
- réglage Throttle pour accélérer / ralentir ;
- choix 2 temps / 4 temps ;
- choix 125 / 250 / 450 cc ;
- volume.

Compilation sans installer de compilateur sur votre PC
------------------------------------------------------
Le workflow GitHub Actions utilise le kit officiel SD88 et compile le binaire ARM dans le cloud.
Il faut toutefois un compte GitHub et importer ce dossier dans un nouveau dépôt GitHub. Aucun mot de passe
ni accès à votre Akai Force n'est demandé à cette étape.

1. Ouvrir https://github.com/new et créer un dépôt nommé MotoCross-Engine-Force.
2. Dans le dépôt vide, choisir « uploading an existing file » (importer des fichiers existants).
3. Extraire ce ZIP sur le PC, puis glisser les fichiers ET le dossier caché .github dans la page GitHub.
   Si le navigateur ne montre pas .github, il faudra activer l'affichage des éléments cachés ou utiliser
   l'option d'importation du dépôt.
4. Valider « Commit changes ».
5. Ouvrir l'onglet Actions, choisir « Build MotoCross Engine for Akai Force », cliquer « Run workflow »,
   garder la version 0.1.0 et lancer.
6. Si la compilation réussit, ouvrir l'exécution terminée et télécharger l'artefact
   MotoCross-Engine-Force-0.1.0. Il contient le ZIP d'installation généré par SD88.

Important
--------
- Le workflow est préparé mais n'a pas été exécuté depuis ce projet : le ZIP installable n'existe donc pas
  encore et je ne prétends pas qu'il est testé sur une Force.
- Le test automatisé vérifie le port sur ordinateur, pas le son sur le matériel réel.
- Ne lancez pas l'installation sur votre Force avant que la compilation soit verte et que le ZIP soit généré.
- Sources et kit officiels : https://github.com/sd88me/mpc-vst-plugins
