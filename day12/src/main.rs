mod part1;
mod part2;
use std::fs::{self,File};
use std::io::{self, BufRead};
use std::iter::repeat;
use rayon::prelude::*;

fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut result1 = 0;
    let mut result2 = 0;
    let mut line_nr = 0;
    for line in input_reader.lines(){
        println!("{}", line_nr);
        line_nr += 1;
        let line_str = line.unwrap();
        let (spring_sym, spring_block) = line_str.split_once(' ').unwrap();
        let spring_groups = spring_block.split(',')
            .map(|group| group.parse::<usize>().unwrap())
            .collect::<Vec<_>>();
        let line_result1 = part1::solve_line(spring_sym, &spring_groups);
        result1 += line_result1;
        let spring_groups2 = repeat(spring_groups.iter()).take(5).flatten().map(|val| *val).collect::<Vec<usize>>();
        let spring_sym2 = repeat(format!("?{}", spring_sym).chars()).take(5).flatten().skip(1).collect::<String>();
        let line_result2 = part2::solve_line(&spring_sym2, &spring_groups2);
        result2 += line_result2;
    }
    println!("{}",result1);
    println!("{}",result2);
}

fn _main() {
    let input_str = fs::read_to_string("resources/input").unwrap();
    let (result1, result2) = input_str.par_lines().map(|line_str|{
        let (spring_sym, spring_block) = line_str.split_once(' ').unwrap();
        let spring_groups = spring_block.split(',')
            .map(|group| group.parse::<usize>().unwrap())
            .collect::<Vec<_>>();
        let line_result1 = part2::solve_line(spring_sym, &spring_groups);
        let spring_groups2 = repeat(spring_groups.iter()).take(5).flatten().map(|val| *val).collect::<Vec<usize>>();
        let spring_sym2 = repeat(format!("?{}", spring_sym).chars()).take(5).flatten().skip(1).collect::<String>();
        let line_result2 = part2::solve_line(&spring_sym2, &spring_groups2);
        (line_result1, line_result2)
    }).collect::<Vec<_>>().iter().fold((0,0), |(sum1, sum2), (val1, val2)| (sum1+val1, sum2+val2));
    println!("{}",result1);
    println!("{}",result2);
}
