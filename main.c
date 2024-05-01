#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

char* cleanWord(const char *rawWord){
    char *word = (char*)malloc((strlen(rawWord) + 1) * sizeof(char));
    strcpy(word, rawWord);
    int removes = 0;
    //remove end punctuation
    //word[strlen(word)-1] = '\0';
    while(word[strlen(word)-1] == ',' || word[strlen(word)-1] == '.'){
        printf("%c\n",  word[strlen(word)-1]);

        word[strlen(word)-1] = '\0';
    }
    //remove rest of punctuation
    for(int x = 0; x < strlen(word); x++){
        printf("%c\n",word[x]);
        if(word[x] == ',' || word[x] == '.'){
            removes++;
            for(int y = 0; y < strlen(word)-x-1; y++){
                word[y+x] = word[y+x+1];
            }
            x--;
        }

    }
    word[strlen(word)-removes] = '\0';
    printf("word done\n");
    return word;
}

int main() {
    FILE *file;
    FILE *counter;
    char line[100];
    char counterLine[100];
    char delimiter[] = " ";
    char words[500][500];
    int wordCounter[500];
    int totalWords = 0;
    int wordIndex = 0;
    int messages = 0;
    char filename[100];
    printf("Please enter the filename for the file to be read:\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    printf("%s", filename);
    file = fopen(filename,"r");
    counter = fopen(filename,"r");
    if(file == NULL){
        perror("Error opening file");
        return 1;
    }
    //printf("Success\n-----------------------\n");
    while(fgets(counterLine, sizeof(counterLine), counter) != NULL){
        messages++;
    }
    int msgPos[100][messages][200];
    int msgLengths[messages];
    for(int x = 0; x < 100; x++){
        for(int y = 0; y < messages; y++){
            for(int z = 0; z < 200; z++){
                msgPos[x][y][z] = -1;
            }
        }
    }
    int msgIndex = 0;
    while(fgets(line, sizeof(line), file) != NULL){
        char *token = strtok(line, delimiter);
        int sentenceIndex = 0;
        while(token != NULL){
            totalWords++;
            //printf("%s\n", token);
            //handles newlines for messages
            if(token[strlen( token)-1] == '\n'){
                //printf("New line detected New Token: ");
                token[strlen(token)-1] = '\0';
                //printf("%s-\n",token);
            }
            if(wordIndex > 0){
                //check if in word list
                bool match = false;
                //char *cleanedToken = cleanWord(token);
                int duplicateIndex = 0;
                for(int x = 0; x < wordIndex; x++){
                    //char *cleanedWord = cleanWord(words[x]);

                    if(strcmp(token, words[x]) == 0){
                        match = true;
                        duplicateIndex = x;
                        x = wordIndex;
                    }
                }
                if(!match){
                    strcpy(words[wordIndex], token);
                    wordCounter[wordIndex] = 1;
                    msgPos[wordIndex][msgIndex][wordCounter[wordIndex]-1] = sentenceIndex;
                    //printf("%s - [%d] [%d] [%d] ~(%d)\n", token, wordIndex, msgIndex, wordCounter[wordIndex]-1, sentenceIndex);
                    wordIndex++;
                }else{
                    //duplicate word; track position
                    //increase duplicate counter
                    wordCounter[duplicateIndex]++;
                    msgPos[duplicateIndex][msgIndex][wordCounter[duplicateIndex]-1] = sentenceIndex;
                    //printf("Duplicate: %s - [%d] [%d] [%d] ~(%d)\n", token, wordIndex, msgIndex, wordCounter[duplicateIndex]-1, sentenceIndex);
                }

            }else{
                //word list is empty; add word
                strcpy(words[wordIndex], token);
                wordCounter[wordIndex] = 1;
                msgPos[wordIndex][msgIndex][wordCounter[wordIndex]-1] = sentenceIndex;
                //printf("%s - [%d] [%d] [%d] ~(%d)\n", token, wordIndex, msgIndex, wordCounter[wordIndex]-1, sentenceIndex);
                wordIndex++;

            }

            //printf("%s, [%s]\n", words[wordIndex-1],words[wordIndex]);
            //printf("\n");


            token = strtok(NULL, delimiter);
            sentenceIndex++;
        }
        msgLengths[msgIndex] = sentenceIndex;
        //printf("%s",line);
        //printf("Message Ended\n---------------------\n---------------------\n");
        msgIndex++;

    }
    //Messages Statistics
    printf("Total Words: %d\nTotal Unique Words: %d\nTotal Messages: %d\n", totalWords, wordIndex, messages);
    printf("---------------------------------------------------------------\n");
    printf("Unique Words:\n--------------------------------\n");
    for(int x = 0; x < wordIndex; x++){
        printf("%s ",words[x]);
    }
    printf("\n---------------------------------------------------------------------------------\n\n");
    bool exit = false;
    while(!exit){

        printf("Which message would you like to see? Enter a number between 1 and %d or enter any key to exit\n", messages);
        char input[100];
        fgets(input, sizeof(input), stdin);
        //reconstructing sentence
        char *endptr;
        errno = 0; // Reset errno before calling strtol
        int number = strtol(input, &endptr, 10);


        if ((errno == ERANGE && (number == LONG_MAX || number == LONG_MIN)) || (errno != 0 && number == 0)) {
            perror("strtol");
            return 1;
        }

        int sentenceNum = number;

        if(sentenceNum > 0 && sentenceNum <= messages){
            sentenceNum--;
            char listOfWords[msgLengths[sentenceNum]][100];
            for(int x = 0; x < wordIndex; x++){
                bool outOfBounds = false;
                int sentenceIndex;
                int county = 0;
                while(!outOfBounds){
                    sentenceIndex = msgPos[x][sentenceNum][county];
                    if(county > 199){
                        outOfBounds = true;
                    }else{
                        //add to sentence list
                        if(sentenceIndex != -1){

                            strcpy(listOfWords[sentenceIndex], words[x]);
                        }
                        county++;
                    }
                }
            }
            printf("-------------------------------------\n");
            for(int x = 0; x < msgLengths[sentenceNum]; x++){
                printf("%s ", listOfWords[x]);
            }
            printf("\n-------------------------------------\n");
        }else if(sentenceNum < 0 || sentenceNum > messages){
            printf("Please enter a valid number between 1 and %d\n", messages);
        }else{
            exit = true;
        }
    }
    //------
    //CODE FOR TESTING
    /*
    for(int x = 0; x < wordIndex; x++){
        if(wordCounter[x] > 1){
            printf("%s(%d) -index(es):",words[x], wordCounter[x]);
            for(int y = 0; y < messages; y++){
                printf(" [");

                for(int z = 0; z < messages; z++){
                    if(msgPos[x][y][z] != -1){
                        printf("%d, ", msgPos[x][y][z]);
                    }

                }
                printf("]");
            }
            printf("\n");
        }


        //printf("------\n");
    }*/

    //printf("Final:%s", words[0]);
    /*
    for(int x = 0; x < messages; x++){
        printf("%d, ",msgLengths[x]);
    }*/

    return 0;
}
