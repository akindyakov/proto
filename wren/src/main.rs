extern crate clap;
use clap::{Arg, App, SubCommand};


extern crate groestl;
use groestl::{Digest, Groestl256};

extern crate hex;

use std::fs::OpenOptions;
use std::io::Write;
use std::path::Path;
use std::time::SystemTime;

fn todo(args: &clap::ArgMatches<>) {
    // TODO: take values from config
    let todo_root_path = Path::new(
        "~/source/git.note"
    );
    let rel_todo_list_path = Path::new(
        "todo/list.md"
    );
    let rel_tasks_dir_path = Path::new(
        "todo/files"
    );
    let todo_list_path = todo_root_path.join(rel_todo_list_path);
    // if args.is_present("message") {
    //     println!(
    //         "Message: {}",
    //         args.values_of("message").unwrap().collect::<Vec<_>>().join(" ")
    //     );
    // }
    // if args.is_present("tag") {
    //     println!(
    //         "Tags: #{}",
    //         args.values_of("tag").unwrap().collect::<Vec<_>>().join(" #")
    //     );
    // }
    if args.is_present("list") {
        println!("List of tasks");
    } else {
        let mut hasher = Groestl256::default();
        let message = args
            .values_of("message")
            .unwrap()
            .collect::<Vec<_>>()
            .join(" ");
        hasher.input(message.into_bytes());
        let tags = args.values_of("tag").unwrap().collect::<Vec<_>>();
        for tag in tags.iter() {
            hasher.input(tag);
        }
        let uid = hex::encode(hasher.result());
        let rel_task_file_path = rel_tasks_dir_path.join(uid + ".md");
        let task_file_path = todo_root_path.join(rel_task_file_path);
        {
            let mut file = OpenOptions::new()
                .create(true)
                .append(true)
                .write(true)
                .open(todo_list_path).unwrap();
            file.write_all(
                format!(
                    " - [{}]({}.md) {}\n",
                    uid[0..6],
                    rel_task_file_path.display(),
                    message
                )
            ).unwrap();
        }
        println!("Create new {} {} {}", uid[0..6], message, rel_task_file_path);
        {
            let mut file = OpenOptions::new()
                .create(true)
                .append(true)
                .write(true)
                .open(task_file_path).unwrap();
            file.write(
                format!("# {} {}\n\n", uid[0..6], message)
            ).unwrap();
            file.write(
                format!(" #{}\n\n", tags.join(" #"))
            ).unwrap();
            file.write(
                format!("[back]({})\n", rel_todo_list_path)
            ).unwrap();
            file.write(
                format!("Created: {}\n\n", SystemTime::now())
            ).unwrap();
        }
    }
}

fn time_log(_args: &clap::ArgMatches<>) {
}

fn test(_args: &clap::ArgMatches<>) {
}

fn main() {
    println!("Hello, world!");
    let args = App::new("wren program")
        .version("0.0.1")
        .author("Alexander Kindyakov <akindyakov@gmail.com>")
        .about("Does awesome things")
        .arg(
            Arg::with_name("config")
                .short("c")
                .long("config")
                .value_name("FILE")
                .help("Sets a custom config file")
                .takes_value(true)
        )
        .arg(
            Arg::with_name("v")
                .short("v")
                .multiple(true)
                .global(true)
                .help("Sets the level of verbosity")
        )
        .subcommand(
            SubCommand::with_name("todo")
                .about("task tracker")
                .arg(
                    Arg::with_name("list")
                        .short("l")
                        .long("list")
                        .help("list all existing tasks")
                )
                .arg(
                    Arg::with_name("tag")
                        .long("tag")
                        .multiple(true)
                        .short("t")
                        .takes_value(true)
                        .help("tag")
                )
                .arg(
                    Arg::with_name("message")
                        .help("message")
                        .last(true)
                        .multiple(true)
                        .raw(true)
                        .takes_value(true)
                )
        )
        .subcommand(
            SubCommand::with_name("test")
                .about("controls testing features")
                .version("1.3")
                .author("Someone E. <someone_else@other.com>")
                .arg(
                    Arg::with_name("debug")
                        .short("d")
                        .help("print debug information verbosely")
                )
        )
        .get_matches();

    // Gets a value for config if supplied by user, or defaults to "default.conf"
    let config = Path::new(
        args.value_of("config").unwrap_or("~/.wren.conf")
    );
    println!("Path to config file: {}", config.display());

    return match args.subcommand() {
        ("todo", Some(sub_args)) => {
            todo(sub_args)
        },
        ("test", Some(sub_args)) => {
            test(sub_args)
        },
        ("log", Some(sub_args)) => {
            time_log(sub_args)
        },
        (_, None) => {
        },
        (_, Some(_)) => {
        },
    }
}
