use std::collections::HashMap;

pub fn solve_line(spring_sym: &str, spring_groups: &Vec<usize>) -> usize{
    let mut matrix: HashMap<(isize, usize, bool), usize> = HashMap::new();
    let mut possible_non_fit = 1;
    for spring_pos in 0..spring_sym.len(){
        if spring_sym.chars().nth(spring_pos).unwrap() == '#'{
            possible_non_fit = 0;
        }
        matrix.insert((spring_pos.try_into().unwrap(), 0, true), 0);
        matrix.insert((spring_pos.try_into().unwrap(), 0, false), possible_non_fit);
    }
    for group_count in 1..spring_groups.len()+1{
        for spring_pos in 0..spring_sym.len(){
            matrix.insert((spring_pos.try_into().unwrap(), group_count, true), calc_possible(spring_pos, group_count, true, spring_sym, spring_groups, &matrix));
            matrix.insert((spring_pos.try_into().unwrap(), group_count, false), calc_possible(spring_pos, group_count, false, spring_sym, spring_groups, &matrix));
        }
    }
    get_possible(sub(spring_sym.len(),1),spring_groups.len(), true, &matrix) + get_possible(sub(spring_sym.len(),1),spring_groups.len(), false, &matrix)
}


fn calc_possible(spring_pos: usize, group_count: usize, last_is_group: bool, spring_sym: &str, spring_groups: &Vec<usize>, matrix: &HashMap<(isize, usize, bool), usize>) -> usize{
    let group_size = *spring_groups.get(group_count-1).unwrap();
    if spring_sym.chars().nth(spring_pos).unwrap() == '.'{
        if last_is_group{
            return 0
        }else {
            return get_possible(sub(spring_pos,1), group_count, true, matrix) + get_possible(sub(spring_pos,1), group_count, false, matrix) 
        }
    }else if spring_sym.chars().nth(spring_pos).unwrap() == '#' {
        if last_is_group {
            if spring_pos+1 >= group_size{
                if !spring_sym[spring_pos+1-group_size..spring_pos+1].chars().any(|sym| sym == '.'){
                    return get_possible(sub(spring_pos, group_size), group_count - 1, false, matrix)
                }
            }
        }
        return 0
    } else{
        if last_is_group{
            if spring_pos+1 >= group_size{
                if !spring_sym[spring_pos+1-group_size..spring_pos+1].chars().any(|sym| sym == '.'){
                    return get_possible(sub(spring_pos, group_size), group_count - 1, false, matrix)
                }
            }
            return 0
        }
        return get_possible(sub(spring_pos,1), group_count, true, matrix) + get_possible(sub(spring_pos,1), group_count, false, matrix) 
    }

}
fn sub(num1: usize, num2:usize) -> isize{
    let inum1: isize = num1.try_into().unwrap();
    let inum2: isize = num2.try_into().unwrap();
    inum1-inum2
}
fn get_possible(spring_pos: isize, group_count: usize, last_is_group:bool, possibility_matrix: &HashMap<(isize, usize, bool), usize>) -> usize{
    if spring_pos < -1 {
        return 0
    }
    if spring_pos == -1  {
        if group_count == 0{
            return 1
        } else{
            return 0
        }
    }
    if let Some(possibilities) = possibility_matrix.get(&(spring_pos, group_count, last_is_group)){
        //println!("{}",possibilities);
        return *possibilities
    }else {
        panic!("Tried to get ({}, {}, {})", spring_pos, group_count, last_is_group)
    }

}
