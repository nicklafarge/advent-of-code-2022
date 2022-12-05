using DataFrames

function main(;sample=true)
    title_label = sample ? "Sample" : "Actual"
    println("---------------- $(title_label) Content ----------------")
    filename = sample ? "sample_contents" : "contents"
    f = open("src/day5/$(filename).txt", "r")

    all_crates = []
    all_crates_p2 = []
    ncrates = sample ? 3 : 9
    for c=1:ncrates
        push!(all_crates, [])
    end
    crate_indices = [2+i*4 for i=0:ncrates-1]
    rearranging=false
    for (i, line) in enumerate(readlines(f))

        if line == ""
            for c in all_crates
                pop!(c)
            end
            rearranging = true
            all_crates_p2 = deepcopy(all_crates)
            continue
        end

        line_chars = collect(line)
        
        if rearranging
            space_split = split(line, " ")
            n_moved = parse(Int32, space_split[2])
            from = parse(Int32, space_split[4])
            to = parse(Int32, space_split[6])
            for i=1:n_moved
                item = popfirst!(all_crates[from])
                pushfirst!(all_crates[to], item)
            end

            items = first(all_crates_p2[from], n_moved)
            all_crates_p2[to] = [items; all_crates_p2[to]]
            all_crates_p2[from] = all_crates_p2[from][n_moved+1:end]
            
        else
            line_chars = collect(line)
            for c=1:ncrates
                crate_val = line_chars[crate_indices[c]]
                if (crate_val != ' ')
                    push!(all_crates[c], crate_val)
                end
            end
        end

    end

    tops = [c[1] for c in all_crates]
    tops2 = [c[1] for c in  all_crates_p2]
    println("Part (1): $(String(tops))")
    println("Part (2):  $(String(tops2))")

    return all_crates
end

all_crates = main(sample=true)

main(sample=false)
println("------------------------------------------------")