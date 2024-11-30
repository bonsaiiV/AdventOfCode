use std::collections::{HashMap, VecDeque};
use std::fs::File;
use std::io::{self, BufRead, Write};

fn main() {
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut graph: HashMap<String, Vec<String>> = HashMap::new();
    let mut goal = "".to_string();
    for line_opt in input_reader.lines() {
        let line = line_opt.unwrap();
        let (node, neighbors) = line.split_once(": ").unwrap();
        let mut neighbor_vec = Vec::new();
        for neighbor in neighbors.split(" ") {
            neighbor_vec.push(neighbor.to_string());
            if let Some(adj) = graph.get_mut(neighbor) {
                adj.push(node.to_string());
            } else {
                graph.insert(neighbor.to_string(), vec![node.to_string()]);
            }
        }
        if let Some(adj) = graph.get_mut(node) {
            adj.append(&mut neighbor_vec);
        } else {
            graph.insert(node.to_string(), neighbor_vec);
        }
        goal = node.to_string();
    }
    let mut count = (0, 0);
    let mut progress = 0;
    for (_start, neighbors) in graph.iter() {
        print!("\r{}/{}", progress, graph.len());
        let _ = io::stdout().flush();
        progress += 1;
        let mut closed_nodes = Vec::new();
        let mut connections_count = 0;
        let mut found_goal = true;
        if neighbors.contains(&goal) {
            connections_count += 1;
        }
        while found_goal {
            found_goal = false;
            let mut used_nodes = Vec::new();
            let mut front = neighbors
                .iter()
                .filter(|&neighbor| *neighbor != goal && !closed_nodes.contains(&neighbor))
                .map(|neighbor| (neighbor, vec![neighbor]))
                .collect::<VecDeque<_>>();
            while let Some((node, mut path)) = front.pop_front() {
                let next_nodes = graph.get(node).unwrap();
                if next_nodes.contains(&goal) {
                    connections_count += 1;
                    front = front
                        .into_iter()
                        .filter(|(_, past_nodes)| {
                            !past_nodes.iter().any(|past_node| path.contains(past_node))
                        })
                        .collect::<VecDeque<_>>();
                    closed_nodes.append(&mut path);
                    found_goal = true;
                } else {
                    let next_nodes_iter = next_nodes
                        .iter()
                        .filter(|next_node| !used_nodes.contains(next_node) && !closed_nodes.contains(next_node))
                        .collect::<Vec<_>>();
                    front.extend(next_nodes_iter.iter().map(|next_node| {
                        let mut new_path = path.clone();
                        new_path.push(next_node);
                        (*next_node, new_path)
                    }));
                    used_nodes.extend(next_nodes_iter);
                }
            }
        }
        if connections_count > 3 {
            count.0 += 1;
        } else {
            count.1 += 1;
        }
    }
    println!("\n{:?}", count);
    println!("{}", count.0 * count.1);
}
