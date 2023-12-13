use std::fs;
use std::collections::{HashSet, HashMap};

fn main() {
    let input_string = fs::read_to_string("resources/input").unwrap();
    let mut result1 = 0;
    let mut pattern_nr = 0;
    for pattern in input_string.split("\n\n"){
        print!("{}: ", pattern_nr);
        pattern_nr += 1;
        let lines_set = pattern.lines()
            .enumerate()
            .fold(HashMap::new(), |mut lines_map: HashMap<String, HashSet<usize>>, (current_line_nr, line)| {
                if let Some(line_nrs) = lines_map.get_mut(line){
                    line_nrs.insert(current_line_nr);
                }else {
                    let lines_set = [current_line_nr].iter()
                        .map(|line_nr| *line_nr)
                        .collect::<HashSet<_>>();
                    lines_map.insert(line.to_string(), lines_set);
                }
                lines_map
            }).iter().map(|(line, line_nr_set)| line_nr_set.clone()).collect::<Vec<_>>();
        let lines_in_pattern = pattern.lines().count();
        if let Some(symmetry) = get_symmetries(&lines_set, lines_in_pattern).iter().next(){
            result1 += symmetry * 100;
            println!("added as horizontal");
        }
        let mut line_len = 0;
        let line_symmetries = pattern.lines().map(|line|{
            let chars_set = line.chars()
                .enumerate()
                .fold(HashMap::new(), |mut chars_map: HashMap<char, HashSet<usize>>, (current_char_nr, char)| {
                    if let Some(char_nrs) = chars_map.get_mut(&char){
                        char_nrs.insert(current_char_nr);
                    }else {
                        let chars_set = [current_char_nr].iter()
                            .map(|char_nr| *char_nr)
                            .collect::<HashSet<_>>();
                        chars_map.insert(char, chars_set);
                    }
                    chars_map
                }).iter().map(|(char, char_nr_set)| char_nr_set.clone()).collect::<Vec<_>>();
            line_len = line.len();
            get_symmetries(&chars_set, line.len()) 

        }).collect::<Vec<HashSet<usize>>>();
        for pos in 0..line_len{
            if line_symmetries.iter().all(|symmetries| symmetries.iter().any(|sym_pos| *sym_pos==pos)){
                println!("added as vertical");
                result1 += pos;
            }
        }
        //println!("{:?}", line_symmetries);
    }
    println!("{}", result1);
}

fn get_symmetries2( pattern_set:&HashMap<String, HashSet<usize>>, pattern_len: usize) -> HashSet<usize>{
    let mut ret = HashSet::new();
    for symmetry_pos_candidate in 0..pattern_len-1{
         if pattern_set.iter().all(|token_nrs| {
            token_nrs.iter().all(|token_nr| {
                if *token_nr <= (symmetry_pos_candidate)*2 
                    && 2*symmetry_pos_candidate-token_nr+1<pattern_len{
                    token_nrs.contains(&(2*symmetry_pos_candidate -token_nr+1))
                }else {
                    true
                }
            })
        }){
            ret.insert(symmetry_pos_candidate+1);
        }
    }
    ret
}

fn get_symmetries( pattern_set:&Vec<HashSet<usize>>, pattern_len: usize) -> HashSet<usize>{
    let mut ret = HashSet::new();
    for symmetry_pos_candidate in 0..pattern_len-1{
         if pattern_set.iter().all(|token_nrs| {
            token_nrs.iter().all(|token_nr| {
                if *token_nr <= (symmetry_pos_candidate)*2 
                    && 2*symmetry_pos_candidate-token_nr+1<pattern_len{
                    token_nrs.contains(&(2*symmetry_pos_candidate -token_nr+1))
                }else {
                    true
                }
            })
        }){
            ret.insert(symmetry_pos_candidate+1);
        }
    }
    ret
}
