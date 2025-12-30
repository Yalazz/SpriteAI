from fastapi import FastAPI
from pydantic import BaseModel
from typing import List, Optional

app = FastAPI(title="SpriteAI AI Service", version="0.1.0")

class Point(BaseModel):
    x: float
    y: float
    pressure: float = 1.0

class Stroke(BaseModel):
    rgba: str = "#ffffffff"
    width: float = 6.0
    points: List[Point]

class Stroke2StrokeRequest(BaseModel):
    stroke: Stroke
    mode: str = "clean"

class Stroke2StrokeResponse(BaseModel):
    strokes: List[Stroke]

@app.get("/health")
def health():
    return {"status": "ok"}

@app.post("/ai/stroke2stroke", response_model=Stroke2StrokeResponse)
def stroke2stroke(req: Stroke2StrokeRequest):
    # Skeleton behavior:
    # - mode="clean": lightly resample points
    # - mode="mirror": outputs mirrored stroke around x=0 for demo
    s = req.stroke

    if req.mode == "mirror":
        mirrored = Stroke(
            rgba=s.rgba,
            width=s.width,
            points=[Point(x=-p.x, y=p.y, pressure=p.pressure) for p in s.points],
        )
        return Stroke2StrokeResponse(strokes=[s, mirrored])

    # "clean": simple downsample
    pts = s.points[::2] if len(s.points) > 2 else s.points
    cleaned = Stroke(rgba=s.rgba, width=s.width, points=pts)
    return Stroke2StrokeResponse(strokes=[cleaned])
