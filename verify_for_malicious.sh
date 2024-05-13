#!/bin/bash

# Verifică dacă un fișier este periculos
check_file() {
    local file="$1"
    local lines=$(wc -l < "$file")
    local words=$(wc -w < "$file")
    local characters=$(wc -c < "$file")
    local keywords=("corrupted" "dangerous" "risk" "attack" "malware" "malicious")
    local has_keyword=false
    local has_non_ascii=false

    while IFS= read -r line; do
        for keyword in "${keywords[@]}"; do
            if [[ $line == *"$keyword"* ]]; then
                has_keyword=true
                break
            fi
        done
        if $has_keyword; then
            break
        fi
    done < "$file"

    if [ "$(file -bi "$file")" != "text/plain; charset=us-ascii" ]; then
        has_non_ascii=true
    fi

    if [[ $lines -gt 1000 || $words -gt 10000 || $characters -gt 100000 || $has_keyword == true || $has_non_ascii == true ]]; then
        echo "Fișier periculos găsit: $file"
    fi
}

# Verifică un director și toate subdirectoarele sale
check_directory() {
    local dir="$1"
    local isolated_dir="$2"
    local files=$(find "$dir" -type f)

    for file in $files; do
        if [[ "$file" != "$isolated_dir"/* ]]; then
            check_file "$file"
        fi
    done

    local subdirs=$(find "$dir" -type d)
    for subdir in $subdirs; do
        if [[ "$subdir" != "$isolated_dir" ]]; then
            check_directory "$subdir" "$isolated_dir"
        fi
    done
}

# Verifică directorul specificat și toate subdirectoarele sale
if [ "$#" -ne 1 ]; then
    echo "Utilizare: $0 [Director de scanat]"
    exit 1
fi

dir_path="$1"
isolated_dir="Isolated"

check_directory "$dir_path" "$isolated_dir"
