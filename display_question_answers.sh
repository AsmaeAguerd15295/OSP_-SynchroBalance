#!/bin/bash

# Define variables and colors
answers_file="./answers.txt"
questions_file="./questions.txt"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

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

index=0
score=0
indexes=($(shuf -i 0-$((${#questions[@]}-1))))
shuffled_questions=()
shuffled_answers=()
for i in "${indexes[@]}"; do
    shuffled_questions+=("${questions[$i]}")
    shuffled_answers+=("${answers[$i]}")
done

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
    read -p "Enter Your answer (number of your choice): " choice

    # Check if choice is the answer
    if [ "${words[((choice-1))]}" = "$ans" ]; then
        echo -e "${GREEN}Correct${NC}"

        # Increment Score
        score=$(( score + (5)))
    else
        echo -e "${RED}Wrong, the correct answer was: $ans.${NC}"
    fi
    index=$(( index+1 ))
    echo
done
echo "Your score is: $score"
