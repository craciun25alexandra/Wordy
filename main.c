#include <time.h>
#include "leaderboard.h"

#define MAX_NUM_OF_WORDS 500
#define CONST 500
char aux[5] = "-----";
char eliminate[30]="";
int processGuess(const char *theAnswer, const char *theGuess)
{
  // indiciu
  char clue[6] = {'-', '-', '-', '-', '-', '\0'};

  // un set de flaguri care arata daca litera din raspuns e folosita in indiciu
  int answerFlags[5] = {0, 0, 0, 0, 0};

  // caut corespondente de litere pe aceeasi pozitie
  for (int i = 0; i < 5; i++)
  {
    if (theGuess[i] == theAnswer[i])
    {
      clue[i] = 'G';
      answerFlags[i] = 1;
      aux[i] = theGuess[i];
    }
  }

  // caut corespondente pe alta pozitie
  for (int i = 0; i < 5; i++)
  {
    if (clue[i] == '-')
    { // nu am litera pe pozitia buna
      int findletter = 0;
      for (int j = 0; j < 5; j++)
      {
        if (answerFlags[j] == 0 && theGuess[i] == theAnswer[j])
        {
          // am o corespondenta dar pe alta pozitie si o pun in indiciu
          clue[i] = 'Y';
          answerFlags[j] = 1;
          findletter = 1;
          break; // inchei loop ul dupa j pentru ca nu vreau mai multe indicii de la aceeasi litera
        }
      }
      if (findletter == 0 && strchr(eliminate, theGuess[i]) == 0){

        strncat(eliminate, theGuess + i, 1);} 
    }
  }
  
  if (strcmp(clue, "GGGGG") != 0)
  {
    printf("Your hint: ");
    printf("%s\n", clue);
    printf("Word to find: %s\n", aux);
    printf("Try not to use: ");
    for(int i = 0; i<strlen(eliminate)-1; i++){
      printf("%c, ", eliminate[i]);
    }
    printf("%c\n", eliminate[strlen(eliminate)-1]);
     
  }
  if (strcmp(clue, "GGGGG") == 0) // daca ai ghicit cuvantul returneaza 1
    return 1;
  return 0;
}

int main()
{

  // iau lista de cuvinte
  char **wordsList = calloc(MAX_NUM_OF_WORDS, sizeof(char *));
  int wordCount = 0;
  char *fiveLetterWord = malloc(6 * sizeof(char));
  FILE *wordsFile = fopen("words.txt", "r");
  FILE *leaderboard = fopen("leaderboard.txt", "a");
  while (fscanf(wordsFile, "%s", fiveLetterWord) != EOF && wordCount < MAX_NUM_OF_WORDS)
  {
    wordsList[wordCount] = fiveLetterWord;
    wordCount++;
    fiveLetterWord = malloc(6 * sizeof(char));
  }
  fclose(wordsFile);

  // incepe jocul
  // se alege un cuvant random
  srand(time(NULL));
  char *answer = wordsList[rand() % wordCount];

  // ai 6 incercari pentru a incerca sa ghicesti cuvantul
  int num_of_guesses = 0;
  int guessed_correctly = 0;
  char *guess = malloc(6 * sizeof(char));

  while (guessed_correctly == 0 && num_of_guesses < 5)
  {
    // introduci o incercare
    printf("Input a 5-letter word and press enter: ");
    scanf("%s", guess);
    if (strlen(guess) > 5 || strlen(guess) < 5)
      printf("Incorrect guess lenght!\n");
    else
    {
      int ok = 0;
      int count = 0;
      while (count < MAX_NUM_OF_WORDS && ok == 0)
      {
        if (strcmp(wordsList[count], guess) == 0)
        {
          ok = 1;
        }
        count++;
      }
      if (ok == 1)
      {
        //printf("You have guessed %s\n", guess);
        num_of_guesses += 1;

        // proceseaza incercarea
        guessed_correctly = processGuess(answer, guess);
      }
      else
        printf("Your word is invalide!\n");
    }
  }

  // afiseaza mesajul de final de joc
  if (guessed_correctly == 1)
  {
    printf("Congratulations! You guessed the correct word in %d times!\n", num_of_guesses);
    printf("Enter your name: ");
    char numePlayer[26];
    time_t t;
    time(&t);
    scanf("%s", numePlayer);
    fprintf(leaderboard, "%s %d %s", numePlayer, num_of_guesses, ctime(&t));
    fclose(leaderboard);
    
  }
  else
  {
    printf("You have used up yours guesses... the correct word is %s\n", answer);
  }
  printf("Want to see the leaderboard? (y/n)\n");
  char *command = malloc(2);
  scanf("%s", command);
  if(strcmp(command, "y")==0){
    printf("-------------------LeaderBoard-------------------\n");
    FILE * leaderboard2 = fopen("leaderboard.txt", "r");
    List *list = CreateList();
    char *line = malloc(CONST), *p;
    while(fgets(line, CONST, leaderboard2)){
      
      if(strchr(line, '\r')) p = strtok(line , "\r");
      if(strchr(line, '\n')) p = strtok(line , "\n");
      Player *p1 = malloc(sizeof(Player));
      p = strtok(line, " ");
      p1->nume = strdup(p);
      p = strtok(NULL, " ");
      p1->incercari = atoi(p);
      p = strtok(NULL, "\n");
      p1->date = strdup(p);
      int aux = FindInList(list, (void*)p1, cmp_lexic, cmp_incercari);
      if (aux != 0){
        AddList(list, p1, cmp_incercari);}
        else{
          free(p1->date);
          free(p1->nume);
          free(p1);
        }
    }
    Node *n = list->head;
    int i = 1;
    while(n){
      printf("%d. ", i);
      Print(n->value);
      n = n->next;
      i++;
    }
  fclose(leaderboard2);
  free(line);
  FreeList(&list, freePlayer);
  }

  for (int i = 0; i < wordCount; i++)
  {
    free(wordsList[i]);
  }
  free(command);
  free(wordsList);
  free(fiveLetterWord);
  free(guess);

  return 0;
}