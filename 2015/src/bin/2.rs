use std::{cmp, fs, str::Lines};

fn part1(lines: Lines) -> i32 {
    let mut ans = 0;

    for line in lines {
        let line: Vec<&str> = line.split('x').collect();
        let h: i32 = line[0].parse().unwrap();
        let w: i32 = line[1].parse().unwrap();
        let l: i32 = line[2].parse().unwrap();

        let mut min_area = h * w;
        min_area = cmp::min(min_area, w * l);
        min_area = cmp::min(min_area, h * l);

        ans += min_area;
        ans += 2 * w * l + 2 * h * w + 2 * l * h;
    }

    ans
}

fn part2(lines: Lines) -> i32 {
    let mut ans = 0;

    for line in lines {
        let line: Vec<&str> = line.split('x').collect();
        let h: i32 = line[0].parse().unwrap();
        let w: i32 = line[1].parse().unwrap();
        let l: i32 = line[2].parse().unwrap();

        let mut min_per = 2 * (w + h);
        min_per = cmp::min(min_per, 2 * (w + l));
        min_per = cmp::min(min_per, 2 * (h + l));

        ans += min_per;
        ans += h * w * l;
    }

    ans
}

fn main() {
    let text = fs::read_to_string("input").unwrap();
    let lines = text.lines();

    println!("Part 1: {}", part1(lines.clone()));
    println!("Part 2: {}", part2(lines));
}
