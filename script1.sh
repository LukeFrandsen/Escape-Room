#!/bin/bash

# Function to check if a month is odd or even
check_month() {
    case $1 in
        Jan|Mar|May|Jul|Sep|Nov)
            echo "$1 is an odd month"
            ;;
        Feb|Apr|Jun|Aug|Oct|Dec)
            echo "$1 is an even month"
            ;;
        *)
            echo "Invalid month"
            ;;
    esac
}

# Function to check if a day is odd or even
check_day() {
    if [ $(($1 % 2)) -eq 0 ]; then
        echo "$1 is an even day"
    else
        echo "$1 is an odd day"
    fi
}

# Main script execution
if [ $# -ne 2 ]; then
    echo "Usage: $0 <Month> <Day>"
    exit 1
fi

month=$1
day=$2

check_month $month
check_day $day
