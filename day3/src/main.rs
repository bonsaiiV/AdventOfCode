use regex::Regex;

use std::io::{self, BufRead};
use std::fs::File;
use std::iter::repeat;

fn main(){
    let re_nrs = Regex::new(r"(\d+)").unwrap();
    let re_nst = Regex::new(r"\*").unwrap();
    let re_check = Regex::new(r"[^\.\d]").unwrap();
    let mut lines = io::BufReader::new(File::open("resources/input").unwrap())
        .lines()
        .map(|o| o.map(|l| format!(".{}.",l)));//l.push('1'); l}));
    let mut current_line = lines.nth(0).unwrap().unwrap();
    let mut prev_line = repeat('.').take(current_line.len()).collect::<String>();
    let mut next_line;
    let mut sum = 0;
    let mut sum2 = 0;
    for line in lines {
        next_line = line.unwrap();
        for mat in re_nrs.find_iter(&current_line) {
            let num = mat.as_str().parse::<usize>().unwrap();
            let rel_range = mat.start()-1 .. mat.end()+1;
            let relevant_chars = &prev_line[rel_range.clone()]
                .chars()
                .chain(next_line[rel_range.clone()].chars())
                .chain(current_line[rel_range].chars())
                .collect::<String>();
            if re_check.is_match(relevant_chars){
                sum += num;
            }
        }

        for mat_ast in re_nst.find_iter(&current_line){
            let rel_numbers:Vec<usize> = re_nrs.find_iter(&prev_line)
                .chain(re_nrs.find_iter(&current_line))
                .chain(re_nrs.find_iter(&next_line))
                .filter(|m| m.range().any(
                    |pos1| (mat_ast.start()-1..mat_ast.end()+1).any(
                        |pos2| pos1==pos2))).map(|m| m.as_str().parse::<usize>().unwrap())
                .collect();
            if rel_numbers.len() == 2 {
                sum2 += rel_numbers.iter().product::<usize>();
            }
                    
        }

        prev_line = current_line;
        current_line = next_line;
    }
    //last line of part 1
    for mat in re_nrs.find_iter(&current_line) {
        let rel_range = mat.start()-1 .. mat.end()+1;
        let relevant_chars = &prev_line[rel_range.clone()]
            .chars()
            .chain(current_line[rel_range].chars())
            .collect::<String>();
        let matched_string = mat.as_str();
        if re_check.is_match(relevant_chars){
            sum += matched_string.parse::<usize>().unwrap();
        }
    }
    //last line of part 2
    for mat_ast in re_nst.find_iter(&current_line){
        let rel_numbers:Vec<usize> = re_nrs.find_iter(&prev_line)
            .chain(re_nrs.find_iter(&current_line))
            .filter(|m| m.range().any(
                |pos1| mat_ast.range().any(
                    |pos2| pos1==pos2))).map(|m| m.as_str().parse::<usize>().unwrap())
            .collect();
        if rel_numbers.len() == 2 {
            sum2 += rel_numbers.iter().product::<usize>();
        }
                
    }
    println!("{}, {}", sum, sum2);
}

