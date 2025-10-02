# Makefile pour le projet MNT
# Compilateur et options
CXX = mpic++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Nom de l'exécutable
EXEC = mnt_program

# Fichiers sources et objets
SOURCES = mnt.cpp fonctions.cpp main.cpp mnt_parallel.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = mnt.h fonctions.h

# Règle principale
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers .cpp en .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJECTS) $(EXEC)

# Règles phony (ne correspondent pas à des fichiers)
.PHONY: all clean help

# Aide
help:
	@echo "Utilisation du Makefile:"
	@echo "  make          - Compile le programme"
	@echo "  make clean    - Supprime les fichiers générés"
	@echo "  make help     - Affiche cette aide"
	@echo ""
	@echo "Exécution manuelle du programme:"
	@echo "  mpirun -np <nb_proc> ./$(EXEC) <fichier_data>"
	@echo ""
	@echo "Exemples:"
	@echo "  mpirun -np 1 ./$(EXEC) data/petit.txt      # Séquentiel"
	@echo "  mpirun -np 2 ./$(EXEC) data/petit.txt      # 2 processus"
	@echo "  mpirun -np 4 ./$(EXEC) data/petit.txt      # 4 processus"
	@echo "  mpirun -np 4 ./$(EXEC) data/jeu_essai.txt  # 4 processus avec autre fichier"
	@echo "  mpirun -np 8 ./$(EXEC) data/alpes.txt      # 8 processus avec gros fichier"