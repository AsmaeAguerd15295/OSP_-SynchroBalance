#!/bin/bash

# Define variables and colors
answers_file="./answers.txt"
questions_file="./questions.txt"
save_file="./progress.txt"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color
MAX_TRIES=3

# Function to save game progress
save_game() {
    echo "$((index)),$score" > "$save_file"
    echo "your current score is: $score."
    echo "Game progress saved."
}

# Function to load game progress
load_game() {
    if [ -f "$save_file" ]; then
        read -r index score < "$save_file"
        echo "Loaded game at question $((index+1)) with score $score."
    else
        echo "No saved game available."
        index=0
        score=0
    fi
}

# Start menu
echo "Welcome to the Quiz Game!"
echo "1. Start New Game"
echo "2. Load Game"
echo "3. Quit"
read -p "Select an option: " option

case $option in
    1)
        index=0
        score=0
        echo "Starting a new game..."
        ;;
    2)
        load_game
        ;;
    3)
        echo "Quitting game."
        exit 0
        ;;
    *)
        echo "Invalid option. Exiting."
        exit 1
        ;;
esac

# Check if files exist and have reading permission
if [ ! -f "$questions_file" ] || [ ! -f "$answers_file" ]; then
    echo "Data Files not found!"
    exit 1
fi

if ! [ -r "$questions_file" ] || ! [ -r "$answers_file" ]; then
    echo "You do not have read access to data files!"
    exit 1
fi

# File exists and we have reading permission - continue...
# Read questions and answers and save lines as an array
questions=()
answers=()
while IFS= read -r line; do
    questions+=("$line")
done < "$questions_file"

while IFS= read -r line; do
    answers+=("$line")
done < "$answers_file"

indexes=($(shuf -i 0-$((${#questions[@]}-1))))
shuffled_questions=()
shuffled_answers=()
for i in "${indexes[@]}"; do
    shuffled_questions+=("${questions[$i]}")
    shuffled_answers+=("${answers[$i]}")
done

tries_left=$MAX_TRIES
for line in "${shuffled_questions[@]}"; do
    # Print Question
    echo -e "${YELLOW}Question $(( index+1 )): $line${NC}"

    # get question answers line
    q_ans="${shuffled_answers[index]}"

    # Extract answer - by separating string by semicolon the redo the same thing with comma
    IFS=';' read -r first_half rest_of_line <<< "$q_ans"
    IFS=',' read -r rest ans <<< "$first_half"
    if [ -z "$ans" ]; then
        ans=$rest
    fi

    # Print answers
    ans_words=$(echo "$q_ans" | sed 's/;/,/g')

    # Split the line into an array based on commas
    IFS=',' read -r -a words <<< "$ans_words"
    words_index=0
    for word in "${words[@]}"; do
        echo "$(( words_index+1 )). $word"
        words_index=$(( words_index+1 ))
    done
    
    correct=0
    while [ $tries_left -gt 0 ]; do
        read -p "Enter Your answer (number of your choice): " choice

        # Check if choice is the answer
        if [ "${words[((choice-1))]}" = "$ans" ]; then
            echo -e "${GREEN}Correct${NC}"

            # Increment Score
            score=$(( score + (5)))
            correct=1
            break
        else
            tries_left=$(( tries_left - 1 ))
            if [ $tries_left -gt 0 ]; then
                echo -e "${RED}Wrong answer. You have $tries_left tries left.${NC}"
            else
                echo -e "${RED}Wrong answer, here is the correct answer: $ans.${NC}"
            fi
        fi
    done

    if [ $correct -eq 0 ]; then
        tries_left=$MAX_TRIES
    fi

    index=$(( index+1 ))
    read -p "Do you want to save and exit? (yes/no): " save_exit
    if [ "$save_exit" = "yes" ]; then
        save_game
        exit 0
    fi
done
echo "Your score is: $score"
