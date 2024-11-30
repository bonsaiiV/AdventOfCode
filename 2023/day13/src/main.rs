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
            .map(|(pos, line)| (pos, line.to_string()))
            .collect::<HashMap<usize, String>>();
        let lines_in_pattern = pattern.lines().count();
        if let Some(symmetry) = get_symmetries2(&lines_set, lines_in_pattern).iter().next(){
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

        }).collect::<Vec<HashMap<usize,usize>>>();
        for pos in 1..line_len{
            if line_symmetries.iter()
                .map(|symmetries| symmetries.get(&pos).unwrap())
                .sum::<usize>()==2{
                println!("added as vertical");
                result1 += pos;
            }
        }
        //println!("{:?}", line_symmetries);
    }
    println!("{}", result1);
}

fn get_symmetries( pattern_set:&Vec<HashSet<usize>>, pattern_len: usize) -> HashMap<usize, usize>{
    let mut ret = HashMap::new();
    for symmetry_pos_candidate in 0..pattern_len-1{
         let smudges = pattern_set.iter().map(|token_nrs| {
            token_nrs.iter().map(|token_nr| {
                if *token_nr <= (symmetry_pos_candidate)*2+1 
                    && 2*symmetry_pos_candidate+1-token_nr<pattern_len{
                    if token_nrs.contains(&(2*symmetry_pos_candidate+1-token_nr)) {0}else{1}
                }else {
                    0
                }
            }).sum::<usize>()
        }).sum::<usize>();
        ret.insert(symmetry_pos_candidate+1, smudges);
        
    }
    ret
}

fn get_symmetries2( pattern_set:&HashMap<usize, String>, pattern_len: usize) -> HashSet<usize>{
    let mut ret = HashSet::new();
    for symmetry_pos_candidate in 0..pattern_len-1{
         if pattern_set.iter().map(|(token_nr, line)| {
            if *token_nr <= (symmetry_pos_candidate)*2+1 
                && 2*symmetry_pos_candidate+1-token_nr<pattern_len{
                let cmp_line = pattern_set.get(&(2*symmetry_pos_candidate+1 -token_nr)).unwrap();
                if *cmp_line == *line{
                    0
                }else {
                    let diff = get_diff(line, cmp_line);
                    /*
                    if diff == 1{
                        println!("{}\n{}", cmp_line, line);
                    }
                    */
                    diff
                }
           }else {
                0
           }
        }).sum::<usize>() == 2{
            ret.insert(symmetry_pos_candidate+1);
        }
    }
    ret
}
fn get_diff(line1: &String, line2: &String) -> usize{
    line1.chars().zip(line2.chars()).map(|(char1, char2)| if char1 == char2 {0}else{1}).sum()
}
