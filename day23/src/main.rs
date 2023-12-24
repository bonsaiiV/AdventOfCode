mod part1;
mod vector;
use crate::vector::{E, N, S, V2, W};
use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::iter::repeat;

struct Marker {
    val: usize,
}
fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut forest_map = Vec::new();
    let mut forest_map2 = Vec::new();
    for line_opt in input_reader.lines() {
        let line = line_opt.unwrap();
        let mut line_vec = Vec::new();
        let mut line_vec2 = Vec::new();
        for sym in line.chars() {
            line_vec.push(part1::MapCell {
                distances: HashMap::new(),
                sym: sym,
            });
            line_vec2.push(match sym {
                '#' => Marker { val: 0 },
                _ => Marker { val: 1 },
            });
        }
        forest_map.push(line_vec);
        forest_map2.push(line_vec2);
    }
    let starting_x = forest_map[0]
        .iter()
        .enumerate()
        .filter(|(_, cell)| cell.sym == '.')
        .next()
        .unwrap()
        .0;
    let starting_pos = V2 {
        x: starting_x as isize,
        y: 0,
    };
    part1::solve(forest_map, starting_pos);
    part2_solve(forest_map2, starting_pos);
}

fn part2_solve(mut forest_map: Vec<Vec<Marker>>, starting_pos: V2) {
    let mut front = Vec::from([(starting_pos, 2, 0)]);
    let mut nodes = vec![(starting_pos)];
    let mut next_id = 3;
    let mut graph = HashMap::from([(2, Vec::new())]);
    let mut goal = 0;
    starting_pos.get_mut(&mut forest_map).unwrap().val = 2;
    while let Some((pos, origin, dist)) = front.pop() {
        let neighbors = [N, E, S, W]
            .into_iter()
            .map(|rel_pos| pos + rel_pos)
            .filter(|&neighbor| {
                let cell_opt = neighbor.get(&forest_map);
                cell_opt.map_or(false, |marker| marker.val == 1)
            })
            .collect::<Vec<_>>();
        if neighbors.len() == 1 {
            if neighbors[0].get(&forest_map).unwrap().val == 1 {
                front.push((neighbors[0], origin, dist + 1));
                neighbors[0].get_mut(&mut forest_map).unwrap().val = origin;
            }
        } else if neighbors.len() == 0 {
            if pos.y == forest_map.len() as isize - 1 {
                goal = next_id;
                graph.insert(next_id, Vec::from([(origin, dist)]));
                graph.get_mut(&origin).unwrap().push((next_id, dist));
                next_id += 1;
            } else {
                for marker in [N, E, S, W]
                    .into_iter()
                    .map(|rel_pos| {
                        (pos + rel_pos)
                            .get(&forest_map)
                            .map_or(0, |marker| marker.val)
                    })
                    .filter(|&marker| marker > 1 && marker != origin)
                {
                    if !graph
                        .get(&origin)
                        .unwrap()
                        .iter()
                        .any(|(node, _)| *node == marker)
                    {
                        graph.get_mut(&origin).unwrap().push((marker, dist + 2));
                        graph.get_mut(&marker).unwrap().push((origin, dist + 2));
                    }
                }
            }
        } else {
            //event!(Level::INFO, "neighbor: {:?}", neighbors);
            for neighbor in neighbors.iter() {
                neighbor.get_mut(&mut forest_map).unwrap().val = next_id;
                front.push((*neighbor, next_id, 1));
                nodes.push(pos);
            }
            pos.get_mut(&mut forest_map).unwrap().val = next_id;
            graph.insert(next_id, Vec::from([(origin, dist)]));
            graph.get_mut(&origin).unwrap().push((next_id, dist));
            next_id += 1;
        }
    }
    println!("Graph with {} nodes created:", graph.len());
    let mut path_map = graph
        .keys()
        .map(|key| *key)
        .zip(repeat(Vec::new()))
        .collect::<HashMap<usize, Vec<(String, usize)>>>();
    let mut front = Vec::from([(2, "".to_string(), 0)]);
    while let Some((node, path, dist)) = front.pop() {
        graph.get(&node).unwrap().iter().enumerate().for_each({
            |(n, (neighbor, edge_len))| {
                if !path_map
                    .get(neighbor)
                    .unwrap()
                    .iter()
                    .any(|path_label| path.starts_with(&path_label.0))
                {
                    let new_path = format!("{}{}", path, n);
                    front.push((*neighbor, new_path.clone(), dist + edge_len));
                    path_map
                        .get_mut(neighbor)
                        .unwrap()
                        .push((new_path, dist + edge_len));
                }
            }
        })
    }
    let result = &path_map
        .get(&goal)
        .unwrap()
        .iter()
        .map(|(_, dist)| *dist)
        .max()
        .unwrap();
    println!("{:?}", result);
}
