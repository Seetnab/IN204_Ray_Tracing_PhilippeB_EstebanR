# IN204_Ray_Tracing_PhilippeB_EstebanR

I)	Analyse du projet:
	1)	Fonctionnalités
		A)	Principales
			-AFFICHER UNE IMAGE
			-Sauvegarder une image
			-Retracer un rayon
			-Fonction de définition de l'espace scénique (position caméra, objets,...)
			-Opérations sur les vecteurs
			-Avoir une bibliothèque d'objets assez riche + extensive
			-Supporter les couleurs, textures, transparences et réflexions
			-Avoir unne bibliothèque avec des classes encapsulant plusieurs moteurs de lancer de rayons
			-Proposer au moins un moteur utilisant le parallélisme
			-Créer un programme permettant la génération d'images permettant d'éditer la configuration de la scène, le moteur utilisé et qui affiche l'image

		B)	Secondaires
			-Avoir plusieurs types de sources lumineuses
			-Avoir un temps de calcul "en temps réel"
			-Prendre en entrée des fichiers Blender
			-Interface graphique

	2)	Librairies
		-XML?
		-Blender?
		-SDL
		-OSPray
		-Visionaray
		-POVray
		-OpenRT
		-OpenCV (affichage d'images)

II)	Architecture:
	1)	Classes
		-SCENE_BASIC_OBJECT
			-OBJECT_BASE_SURFACE
				-transparente, texturée, ...
		-RAYON (vecteur: un point + une direction)
		-COULEUR (vecteur)
		-POINT(x,y,z)
		-CAMERA

	2)	Fichiers


Liens utiles:
Tuto Ray Tracing: https://raytracing.github.io/books/RayTracingInOneWeekend.html
Wikipedia RT: https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#Detailed_description_of_ray_tracing_computer_algorithm_and_its_genesis
