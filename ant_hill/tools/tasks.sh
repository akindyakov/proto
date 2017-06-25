#!/usr/bin/env bash

task="${1}"
task_title="${@}"
todo_index="todo/index.md"
tasks_dir="todo/tasks"

case "$1" in
("create")
    task_file="${tasks_dir}/z_$(echo ${task_title} | sha1sum - | head -c 8).md"
    if [[ -f "${task_file}" ]]; then
        echo "Error: task file [${task_file}] exists." >&2
        exit 1
    fi
    echo "Create task '${@}' [${task_file}]" >&2
    echo "[ ] [${task_file}] ${task_title}" >> "${todo_index}"
    echo "$(date +'%F %T') ${task_title}" >> "${task_file}"
;;
("")
    echo "default"
;;
esac
