from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI(
    title="Sprite AI - AI Service",
    version="0.0.1",
    description="Sprite AI için FastAPI tabanlı yapay zeka servis iskeleti."
)

class PixelGenerateRequest(BaseModel):
    prompt: str
    width: int = 64
    height: int = 64

class PixelGenerateResponse(BaseModel):
    message: str

@app.get("/health")
async def health():
    return {"service": "SpriteAI AI Service", "status": "UP"}

@app.post("/api/ai/pixel/generate", response_model=PixelGenerateResponse)
async def generate_pixel_art(request: PixelGenerateRequest):
    # Buraya gerçek model entegrasyonu eklenecek.
    return PixelGenerateResponse(
        message=f"Model henüz bağlı değil. Prompt: '{request.prompt}', boyut: {request.width}x{request.height}"
    )
