import pyautogui
import threading
from eye_tracking import run_relative_eye_tracking
from pipe_server import start_pipe_server
def main():
    # stop_event = threading.Event()
    # start_pipe_server(stop_event=stop_event)
    screen_w, screen_h = pyautogui.size()
    run_relative_eye_tracking(screen_w, screen_h)
    

if __name__ == "__main__":
    main()
