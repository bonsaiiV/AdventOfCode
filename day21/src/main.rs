mod vector;
use crate::vector::{E, N, S, V2, W};
use std::collections::HashSet;
use std::fs;
use std::iter::repeat;

fn main() {
    let input_str = fs::read_to_string("resources/input4").unwrap();
    let steps = 26501365;
    let mut line_nr = 0;
    let mut starting_pos = V2 { x: 0, y: 0 };
    let mut rocks: HashSet<V2> = HashSet::new();
    let mut width = 0;
    let mut field: Vec<Vec<(usize, usize)>> = Vec::new();
    for line in input_str.lines() {
        let mut line_vec = Vec::new();
        let mut char_nr = 0;
        for sym in line.chars() {
            match sym {
                'S' => {
                    starting_pos = V2 {
                        y: line_nr,
                        x: char_nr,
                    };
                    line_vec.push((0, 0));
                }
                '#' => {
                    rocks.insert(V2 {
                        y: line_nr,
                        x: char_nr,
                    });
                    line_vec.push((0, 1));
                }
                _ => line_vec.push((0, 0)),
            };
            char_nr += 1;
        }
        field.push(line_vec);
        width = char_nr;
        line_nr += 1;
    }
    let field_size = V2 {
        x: width,
        y: line_nr,
    };
    let mut front = Vec::from([starting_pos]);
    let mut dist = 0;
    while !front.is_empty() {
        let mut new_front = Vec::new();
        for pos in front.into_iter() {
            field[pos.y as usize][pos.x as usize].0 = dist;
            for mov in [N, E, S, W] {
                let can = pos + mov;
                if can.is_on(&field_size) {
                    if field[can.y as usize][can.x as usize].1 == 0 {
                        field[can.y as usize][can.x as usize].1 = 2;
                        new_front.push(can);
                    }
                }
            }
        }
        dist += 1;
        front = new_front;
    }
    /*
    for y in 0..field_size.y{
        for x in 0..field_size.x{
            if field[y as usize][x as usize].1 == 2{
                print!(".");//{}", field[y as usize][x as usize].0 % 2);
            }else{print!("#");}
        }
        println!();
    }
        */
    let free_even = field
        .iter()
        .flat_map(|vec| vec.iter())
        .filter(|(dist, marker)| *marker == 2 && dist % 2 == 0)
        .count() as isize;
    let free_odd = field
        .iter()
        .flat_map(|vec| vec.iter())
        .filter(|(dist, marker)| *marker == 2 && dist % 2 == 1)
        .count() as isize;
    let super_size = (steps-65) / field_size.x;
    println!("{}", super_size);
    let mut result2 = 0;
    for x in 1..super_size +1 {
        if x % 2 == 1 {
            result2 += 4
                * ((super_size - x + 1) / 2 * free_odd as isize
                    + ((super_size - x) / 2 + 1) * free_even as isize);
        } else {
            result2 += 4
                * ((super_size - x + 1) / 2 * free_even as isize
                    + ((super_size - x + 0) / 2 + 1) * free_odd as isize);
        }
    }
    result2 += free_odd;
    let even_corners = (0..field_size.y)
            .flat_map(|y| (0..field_size.x).zip(repeat(y)))
            .filter(|&(x, y)| {
                field[y as usize][x as usize].1 == 2
                    && (V2 { x: x, y: y } - starting_pos).abs() >= starting_pos.x
                    && (x+y)%2 == 0
            }).collect::<Vec<_>>();
    let even_corners_count = even_corners.len() as isize;
    let odd_corners = (0..field_size.y)
            .flat_map(|y| (0..field_size.x).zip(repeat(y)))
            .filter(|&(x, y)| {
                field[y as usize][x as usize].1 == 2
                    && (V2 { x: x, y: y } - starting_pos).abs() >= starting_pos.x
                    && (x+y)%2 == 1
            }).collect::<Vec<_>>();
    for y in 0..field_size.y{
        for x in 0..field_size.x{
            if odd_corners.contains(&(x, y)){
                print!(".");//{}", field[y as usize][x as usize].0 % 2);
            }else{print!("#");}
        }
        println!();
    }
    let odd_corners_count = odd_corners.len() as isize;
    let corner_diff;
    let missing_corners;
    if super_size%2==1 {
        corner_diff = odd_corners_count - even_corners_count;
        missing_corners = even_corners_count;
    } else {
        corner_diff = even_corners_count - odd_corners_count;
        missing_corners = odd_corners_count;
    }
    result2 += (super_size + 0 ) * corner_diff - missing_corners;
    /*result2 += free_odd as isize - free_even as isize
        + (0..field_size.y)
            .flat_map(|y| (0..field_size.x).zip(repeat(y)))
            .filter(|&(x, y)| {
                field[y as usize][x as usize].1 == 2
                    && (V2 { x: x, y: y } - starting_pos).abs() <= starting_pos.x
            })
            .count() as isize;
    */
    let correct_res:isize = 625587097150084;
    println!("free even: {}, free odd: {}, diff: {}", free_even, free_odd, free_even-free_odd);
    println!("corners even: {}, corners odd: {}, diff: {}", even_corners_count, odd_corners_count, even_corners_count-odd_corners_count);
    println!("{}", result2);
    let error5 = result2 - correct_res;
    println!("Error: {}", error5);
    let rel_error5 = error5 / (super_size + 1);
    let rel_error_free_even = error5/ free_even;
    println!("Relative Errors: super_size: {}, free even: {}", rel_error5, rel_error_free_even);
}
