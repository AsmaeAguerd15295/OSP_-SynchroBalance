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
    echo "$score" > "$save_file"
    echo "Your current score is: $score."
    echo "Game progress saved."
}

load_game() {
    if [ -f "$save_file" ]; then
        read -r score < "$save_file"
        if [ -n "$score" ]; then
            echo "Loaded game with score $score."
        else
            echo "Failed to load game properly. Starting a new game instead."
            score=0
        fi
    else
        echo "No saved game available."
        score=0
    fi
    index=0  # Reset index to start from the first question
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

# Check file existence and reading permission
if [ ! -f "$questions_file" ] || [ ! -f "$answers_file" ]; then
    echo "Data Files not found!"
    exit 1
fi

if ! [ -r "$questions_file" ] || ! [ -r "$answers_file" ]; then
    echo "You do not have read access to data files!"
    exit 1
fi

# Load questions and answers into arrays
# Load questions and answers into arrays
questions=()
answers=()
while IFS= read -r line; do
    questions+=("$line")
done < "$questions_file"

while IFS= read -r line; do
    answers+=("$line")
done < "$answers_file"

# Shuffle indices and use them to access questions and answers
indexes=($(shuf -i 0-$((${#questions[@]} - 1))))
tries_left=$MAX_TRIES
for i in "${indexes[@]}"; do
    echo -e "${YELLOW}${questions[i]}${NC}"
    q_ans="${answers[i]}"

    # Clear previous answers before adding new ones
    unset all_answers
    declare -a all_answers

    # Split answers into correct and incorrect parts
    IFS=';' read -r correct_answer all_incorrect <<< "$q_ans"
    IFS=',' read -r -a all_answers <<< "$all_incorrect"
    all_answers+=("$correct_answer")  # Append correct answer to the list of options

    # Clear previous shuffle before new shuffle
    unset shuffled_indexes
    shuffled_indexes=($(shuf -i 0-$((${#all_answers[@]} - 1))))

    # Clear and declare the associative array anew for each question
    unset index_to_answer
    declare -A index_to_answer
    correct_index=0

    # Assign shuffled answers to a sorted index
    for j in "${!shuffled_indexes[@]}"; do
        idx="${shuffled_indexes[j]}"
        index_to_answer[$((j + 1))]="${all_answers[idx]}"
        if [[ "${all_answers[idx]}" == "$correct_answer" ]]; then
            correct_index=$((j + 1))
        fi
    done

    # Display answers maintaining numerical order
    for k in $(seq 1 ${#index_to_answer[@]}); do
        echo "$k. ${index_to_answer[$k]}"
    done
    echo "$(( ${#index_to_answer[@]} + 1 )). Skip this question"


    correct=0
    tries_left=$MAX_TRIES
    while [ $tries_left -gt 0 ]; do
        read -p "Enter Your answer (number of your choice): " choice

        if [ "$choice" -eq "$(( ${#index_to_answer[@]} + 1 ))" ]; then
            echo -e "${YELLOW}Skipping this question.${NC}"
            break  # Exit the loop and move to the next question without penalty
        elif [ "$choice" -eq "$correct_index" ]; then
            echo -e "${GREEN}Correct${NC}"
            score=$(( score + 5 ))
            correct=1
            break
        else
            tries_left=$(( tries_left - 1 ))
            if [ $tries_left -gt 0 ]; then
                echo -e "${RED}Wrong answer. You have $tries_left tries left.${NC}"
            else
                echo -e "${RED}Wrong answer, the correct answer is: $correct_answer.${NC}"
            fi
        fi
    done

    if [ $correct -eq 0 ]; then
        tries_left=$MAX_TRIES
    fi

    index=$((i + 1))
    read -p "Do you want to save and exit? (yes/no): " save_exit
    if [ "$save_exit" = "yes" ]; then
        save_game
        exit 0
    fi
done
echo "Your score is: $score"
