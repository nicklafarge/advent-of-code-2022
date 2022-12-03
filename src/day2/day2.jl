using DataFrames

struct RPS
    name::String
    id::Int64
    value::Int64
end

rock = RPS("Rock", 0, 1)
paper = RPS("Paper", 1, 2)
scissors = RPS("Scissors", 2, 3)
rps_dict = Dict(0=>rock, 1=>paper, 2=>scissors)

@enum Result win=6 tie=3 loss=0
function compete(opponent::RPS, me::RPS)

    if opponent == me
        result = tie
    elseif((opponent.id + 1) % 3 == me.id)
        result = win
    else
        result = loss
    end

    shape_score = me.value
    outcome_score = Int64(result)
    total_score = shape_score + outcome_score
    return (result, total_score)
end

function create_opponent_throw(symbol::String)
    if symbol == "A"
        return rock
    elseif symbol == "B"
        return paper
    elseif symbol == "C"
        return scissors
    end
end

function create_throw_round1(opponent_throw::RPS, my_symbol::String)
    if my_symbol == "X"
        return rock
    elseif my_symbol == "Y"
        return paper
    elseif my_symbol == "Z"
        return scissors
    end
end

function create_throw_round2(opponent_throw::RPS, my_symbol::String)
    if my_symbol == "X" # Loss
        return rps_dict[(opponent_throw.id+2)%3]
    elseif my_symbol == "Y" # Tie
        return opponent_throw
    elseif my_symbol == "Z" # Win
        return rps_dict[(opponent_throw.id+1)%3]
    end
end



# Open the input file
# f = open("src/day2/day2_sample_strategy.txt", "r")
f = open("src/day2/day2_strategy.txt", "r")
df_round1 = DataFrame(Opponent=RPS[], Me=RPS[], Result=Result[], TotalScore=Int64[])
df_round2 = DataFrame(Opponent=RPS[], Me=RPS[], Result=Result[], TotalScore=Int64[])

for line in readlines(f)
    throw = split(line, " ")
    op_throw = create_opponent_throw(String(throw[1]))

    me1 = create_throw_round1(op_throw, String(throw[2]))
    res1, total_score_1 = compete(op_throw, me1)
    push!(df_round1, [op_throw, me1, res1, total_score_1])

    me2 = create_throw_round2(op_throw, String(throw[2]))
    res2, total_score2 = compete(op_throw, me2)
    push!(df_round2, [op_throw, me2, res2, total_score2])
end

println("Total score (1): $(sum(df_round1.TotalScore))")
println("Total score (2): $(sum(df_round2.TotalScore))")