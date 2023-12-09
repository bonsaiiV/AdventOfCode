use std::io::{self, BufRead};
use std::fs::File;

fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut result1 = 0;
    let mut result2 = 0;
    for line in input_reader.lines() {
        if let Ok(line_str) = line {
            let mut sequences = Vec::new();
            sequences
                .push(line_str.split(" ")
                    .map(|num| num.parse::<isize>().unwrap())
                    .collect::<Vec<isize>>());
            while sequences.last().unwrap().iter().any(|val| *val!=0){
                let mut next_sequence = Vec::new();
                let mut last_seq = sequences.last().unwrap().iter();
                let first_val = last_seq.next().unwrap();
                last_seq.fold(first_val, |prev,next| {next_sequence.push(*next-*prev);next});
                sequences.push(next_sequence);
            }
            result1 += sequences.iter().map(|seq| seq.last().unwrap()).sum::<isize>();
            let firsts = sequences.iter().map(|seq| seq.get(0).unwrap()).collect::<Vec<&isize>>();
            result2 += firsts.iter().rev().fold(0,|prev,next| *next-prev);
            //let mut last_val = 0;
            //for i in (0..sequences.len()){
            //    let this_val = sequences.get(i).unwrap().last();
            //    sequences[i].push(last_val + this_val);
            //    last_val = this_val;
            // }
        }
    }
    println!("{}", result1);
    println!("{}", result2);
}
