use std::fs;

fn part1(text: &str) -> i32 {
    let mut cur = 0;
    for c in text.chars() {
        if c == '(' {
            cur += 1
        } else {
            cur -= 1
        }
    }
    return cur;
}

fn part2(text: &str) -> i32 {
    let mut cur = 0;
    for (i, c) in text.chars().enumerate() {
        if c == '(' {
            cur += 1
        } else {
            cur -= 1
        }

        if cur < 0 {
            return i as i32 + 1;
        }
    }

    return -1;
}

fn main() {
    let text = fs::read_to_string("input").unwrap();

    println!("Part 1: {}", part1(&text));
    println!("Part 2: {}", part2(&text));
}
