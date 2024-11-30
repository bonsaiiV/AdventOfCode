use crate::{E, N, S, V2, W};
use std::collections::HashMap;
#[derive(Debug)]
pub struct MapCell {
    pub distances: HashMap<String, usize>,
    pub sym: char,
}
pub fn solve(mut forest_map: Vec<Vec<MapCell>>, starting_pos: V2) {
    let mut front = Vec::from([(starting_pos, "".to_string(), 0)]);
    while let Some((pos, route_label, dist)) = front.pop() {
        let dirs = [N, E, S, W]
            .into_iter()
            .filter(|&dir| {
                let neighbor = pos + dir;
                let cell_opt = neighbor.get(&forest_map);
                cell_opt.map_or(false, |cell| {
                    cell.sym != '#'
                        && !cell
                            .distances
                            .keys()
                            .any(|prev_route| route_label.starts_with(prev_route))
                        && cell.sym
                            != match dir {
                                N => 'v',
                                E => '<',
                                S => '^',
                                W => '>',
                                _ => ' ',
                            }
                })
            })
            .collect::<Vec<_>>();
        let neighbors = dirs.into_iter().map(|dir| dir + pos).collect::<Vec<_>>();
        if neighbors.len() == 1 {
            let next = neighbors[0];
            next.get_mut(&mut forest_map)
                .unwrap()
                .distances
                .insert(route_label.clone(), dist + 1);
            front.push((next, route_label, dist + 1))
        } else {
            //event!(Level::INFO, "neighbor: {:?}", neighbors);
            for (n, neighbor) in neighbors.iter().enumerate() {
                let new_path = format!("{}{}", route_label, n);
                neighbor
                    .get_mut(&mut forest_map)
                    .unwrap()
                    .distances
                    .insert(new_path.clone(), dist + 1);
                front.push((neighbor.clone(), new_path, dist + 1));
            }
        }
    }
    let result1 = &forest_map
        .last()
        .unwrap()
        .iter()
        .filter(|cell| cell.sym == '.')
        .next()
        .unwrap()
        .distances
        .values()
        .max()
        .unwrap();
    /*for (path, len) in result1.iter(){
        println!("{}:", len);
        print_path(&forest_map, path);
        println!();
    }*/
    println!("{:?}", result1);
}
fn print_path(map: &Vec<Vec<MapCell>>, path_label: &String) {
    for line in map {
        for cell in line {
            if cell
                .distances
                .keys()
                .any(|path| path_label.starts_with(path))
            {
                print!("1");
            } else {
                print!("{}", cell.sym);
            }
        }
        println!();
    }
}
