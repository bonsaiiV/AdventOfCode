use itertools::Itertools;

pub fn solve_line(spring_sym: &str, spring_groups: &Vec<usize>) -> usize{
    let working_springs = spring_sym.chars()
        .filter(|sym| *sym == '#')
        .count();
    let unknown_springs = spring_sym.chars()
        .enumerate()
        .filter(|(_, symbol)| *symbol == '?')
        .map(|(index,_)| index)
        .collect::<Vec<usize>>();
    let total_springs = spring_groups.iter().sum();
    if working_springs + unknown_springs.len() >= total_springs{
        unknown_springs.iter()
            .combinations(total_springs-working_springs)
            .map(|new_working_springs|
                spring_sym
                .chars()
                .enumerate()
                .map( move |(pos, sym)| {
                    if sym == '?' {
                        if new_working_springs.contains(&&pos){
                            '#'
                        }else {
                            '.'
                        }
                    }
                    else {
                        sym
                    }
                })
            ).map(|spring_sym_iter|
                spring_sym_iter.fold((0, Vec::new()), |(chain_of_working, mut groups), sym|{
                    if sym == '#'{
                        (chain_of_working + 1, groups)
                    }
                    else{
                        groups.push(chain_of_working);
                        (0, groups)
                    }
                })
            )
            .map(|(chain, mut groups)|{ 
                groups.push(chain);
                groups.iter().filter(|group_size| **group_size != 0).to_owned().map(|group| *group).collect::<Vec<usize>>()
            })
            .filter(|groups| *groups == *spring_groups)
            .count()
    }
    else {0}
}
