use std::fs::File;
use std::io::{self, BufRead};
use itertools::Itertools;
use std::collections::HashMap;

fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut line_nr = 0;
    let lines = input_reader.lines();
    let mut width = 0;
    let mut static_rock_pos: Vec<(usize, usize)> = Vec::new();
    let mut moving_rock_pos: Vec<(usize, usize)> = Vec::new();
    for line in lines{
        let line_str = line.unwrap();
        width = line_str.chars().fold(0, |char_pos, sym|{
            match sym {
                '#' => static_rock_pos.push((char_pos, line_nr)),
                'O' => moving_rock_pos.push((char_pos, line_nr)),
                _ => (),
            };
            char_pos +1
        });
        line_nr += 1;
    }
    let mut field_size = (width, line_nr);
    //let mut known_fields: Vec<(usize, Vec<(usize, usize)>)> = Vec::new();
    let mut known_fields: HashMap<Vec<(usize, usize)>, usize> = HashMap::new();
    let mut cycle_offset = 0;
    let mut cycle_len = 0;
    let mut goal: Option<usize> = None;
    let static_positions = [
        (0..field_size.0).map(|column_nr| {
        let mut static_column = static_rock_pos.iter()
            .filter(|&&(x, _)| x == column_nr)
            .map(|&(_, y)| y as isize)
            .collect::<Vec<isize>>();
        static_column.push(-1);
        static_column.sort();
        static_column.push(field_size.1 as isize);
        static_column
    }).collect::<Vec<_>>(),
        (0..field_size.1).map(|column_nr| {
        let mut static_column = static_rock_pos.iter()
            .filter(|&&(_, y)| y == column_nr)
            .map(|&(x, _)| x as isize)
            .collect::<Vec<isize>>();
        static_column.push(-1);
        static_column.sort();
        static_column.push(field_size.0 as isize);
        static_column
    }).collect::<Vec<_>>(),
        (0..field_size.0).map(|column_nr| {
        let mut static_column = static_rock_pos.iter()
            .filter(|&&(x, _)| x == column_nr)
            .map(|&(_, y)| (field_size.1-1-y) as isize)
            .collect::<Vec<isize>>();
        static_column.push(-1);
        static_column.sort();
        static_column.push(field_size.1 as isize);
        static_column
    }).collect::<Vec<_>>(),
        (0..field_size.1).map(|column_nr| {
        let mut static_column = static_rock_pos.iter()
            .filter(|&&(_, y)| y == column_nr)
            .map(|&(x, _)| (field_size.0-1-x) as isize)
            .collect::<Vec<isize>>();
        static_column.push(-1);
        static_column.sort();
        static_column.push(field_size.0 as isize);
        static_column
    }).collect::<Vec<_>>()];
    for i in 0..100000 {
        if let Some(goal_pos) = goal{
            if (i-cycle_offset)%cycle_len == goal_pos{
                break;
            }
        }
        if i % 4 == 0{
            //println!("Positions: {:?}", moving_rock_pos);
            //if let Some((offset, _)) = known_fields.iter().filter(|(_, field)| moving_rock_pos.iter().all(|pos| field.contains(pos))).next(){
            if let Some(offset) = known_fields.get(&moving_rock_pos){//.iter().filter(|(_, field)| moving_rock_pos.iter().all(|pos| field.contains(pos))).next(){
                cycle_offset = *offset;
                cycle_len = i-*offset;
                goal = Some((4000000000-cycle_offset)%cycle_len)
            }
            //known_fields.push((i,moving_rock_pos.clone()));
            known_fields.insert(moving_rock_pos.clone(), i);
        }
        //print_field(&moving_rock_pos, &static_rock_pos, &field_size);
        moving_rock_pos = do_step(&static_positions[i%4], &moving_rock_pos, &field_size);
        //println!("{:?}", moving_rock_pos);
        field_size = (field_size.1, field_size.0);
        static_rock_pos = static_rock_pos.iter().map(|pos| rotate(pos, field_size.0)).collect();
        //let result1: usize = moving_rock_pos.iter().map(|(_,y)| field_size.0-y).sum();
        //println!("{}", result1);
    }

    println!("cycle_len: {}, {}",cycle_len, cycle_offset);
    print_field(&moving_rock_pos, &static_rock_pos, &field_size);
    let result1: usize = moving_rock_pos.iter().map(|(_,y)| field_size.0-y).sum();
    println!("{}", result1);
}

fn rotate(tuple: &(usize,usize), field_height: usize) -> (usize,usize){
    (field_height - tuple.1 -1, tuple.0)
}

fn do_step(static_rock_pos: &Vec<Vec<isize>>, current_rock_positions: &Vec<(usize, usize)>, field_size: &(usize,usize)) -> Vec<(usize,usize)>{
    let mut new_rock_positions: Vec<(usize,usize)> = Vec::new();
    for column in 0..field_size.0{
        /*let mut static_xs = static_rock_pos.iter().filter(|(x, _)| *x==column).map(|(_, y)| (*y).try_into().unwrap()).collect::<Vec<isize>>();
        static_xs.push(-1);
        static_xs.sort();
        static_xs.push(field_size.1.try_into().unwrap());*/
        let static_xs=&static_rock_pos[column];
        let column_rocks = current_rock_positions.iter().filter(|&&(x, _)| x==column);
        static_xs.iter().tuple_windows().for_each(|(&low, &high)| {
            new_rock_positions.extend(
                column_rocks.clone()
                .filter(|&&(x,y)| (low < y as isize) & (high >= y as isize))
                .enumerate()
                .map(|(index,_)| (field_size.1-1-((low+1) as usize+index), column)));
        });
    }
    new_rock_positions
}

fn print_field(moving: &Vec<(usize,usize)>, static_rocks: &Vec<(usize,usize)>, field_size: &(usize,usize)){
    for line in 0..field_size.1{
        for sym in 0..field_size.0{
            if static_rocks.contains(&(sym, line)){
                print!("#");
            }else if moving.contains(&(sym, line)){
                print!("O");
            }else {
                print!(".");
            }
        }
        println!();
    }
    println!();
}

