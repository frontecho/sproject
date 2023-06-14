import time

def focus_clock(minutes):
    start_time = time.time()
    end_time = start_time + (minutes * 60)
    
    while time.time() < end_time:
        remaining_time = int(end_time - time.time())
        minutes = remaining_time // 60
        seconds = remaining_time % 60
        timer_string = f"{minutes:02d}:{seconds:02d}"
        
        print(f"Focus Clock: {timer_string}", end="\r")
        time.sleep(1)
    
    print("\nTime's up! Your focus session has ended.")

def save_to_file(content):
    filename = "focus_clock.txt"
    with open(filename, "w") as file:
        file.write(content)
    print(f"The focus clock has been saved to '{filename}'.")

# Set the duration of the focus session in minutes
session_duration = 25

# Run the focus clock
focus_clock(session_duration)

# Save the focus clock to a file
save_to_file(focus_clock)
