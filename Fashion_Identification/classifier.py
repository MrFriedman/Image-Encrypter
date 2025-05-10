"""
Dylan Friedman - FRDDYL002
May 2025
ML Assignment 1 - CSC3022F
"""
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from torchvision import datasets, transforms, io
from torchvision.io import ImageReadMode
import matplotlib.pyplot as plt
import time
import sys
from PIL import Image

# Define the convolutional neural network architecture
class FashionCNN(nn.Module):
    def __init__(self):
        super(FashionCNN, self).__init__()
        
        # Convolutional layers
        self.conv_layers = nn.Sequential(
            # First convolutional block
            nn.Conv2d(1, 32, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(32),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Dropout(0.25),
            
            # Second convolutional block
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(64),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Dropout(0.25),
            
            # Third convolutional block
            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.BatchNorm2d(128),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Dropout(0.25)
        )
        
        # Fully connected layers
        self.fc_layers = nn.Sequential(
            nn.Linear(128 * 3 * 3, 512),
            nn.ReLU(),
            nn.BatchNorm1d(512),
            nn.Dropout(0.5),
            nn.Linear(512, 10)  # 10 output classes
        )
    
    def forward(self, x):
        # Pass input through convolutional layers
        x = self.conv_layers(x)
        
        # Flatten the output for the fully connected layers
        x = x.view(x.size(0), -1)
        
        # Pass through fully connected layers
        x = self.fc_layers(x)
        
        return x

# Class labels for FashionMNIST (found on github)
class_labels = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat',
               'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']

# Function to train the model (define the number of epochs)
def train_model(model, train_loader, val_loader, cost, optimizer, num_epochs, no_improvement=5):
    best_accuracy = None
    best_model = None
    best_epoch = -1
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    print(f"Training on {device}")
    model.to(device)
    
    train_loss = []
    validation_acc = []
    
    for epoch in range(num_epochs):
        # Training
        model.train()
        epoch_loss = []
        start_time = time.time()
        
        for inputs, labels in train_loader:
            inputs, labels = inputs.to(device), labels.to(device)
            
            # Zero the parameter gradients (clear previous gradients)
            optimizer.zero_grad()
            
            # Forward pass - compute the outputs
            outputs = model(inputs)
            
            # Calculate loss
            loss = cost(outputs, labels)
            
            # Backward pass - compute gradients
            loss.backward()
            
            # Update parameters
            optimizer.step()
            
            epoch_loss.append(loss.detach())
        
        # Calculate average loss for the epoch
        avg_loss = torch.tensor(epoch_loss).mean().item()
        train_loss.append(avg_loss)
        
        epoch_time = time.time() - start_time
        
        # Validation
        model.eval()
        val_correct = 0
        val_total = 0
        
        with torch.no_grad():
            for inputs, labels in val_loader:
                inputs, labels = inputs.to(device), labels.to(device)
                outputs = model(inputs)
                _, predicted = torch.max(outputs.data, 1)
                val_total += labels.size(0)
                val_correct += (predicted == labels).sum().item()
        
        val_accuracy = 100 * val_correct / val_total
        validation_acc.append(val_accuracy)
        
        print(f'Epoch {epoch+1}/{num_epochs}, '
              f'Loss: {avg_loss:.4f}, '
              f'Validation Accuracy: {val_accuracy:.2f}%, '
              f'Time: {epoch_time:.2f}s')
        
        # Save the best model in memory
        if best_accuracy is None or val_accuracy > best_accuracy:
            print(f"New best epoch {epoch}, acc {val_accuracy:.2f}%")
            best_accuracy = val_accuracy
            best_model = model.state_dict().copy()
            best_epoch = epoch
        
        # Early stopping check
        if best_epoch + no_improvement <= epoch:
            print(f"No improvement for {no_improvement} epochs")
            break
            
    print(f'Best validation accuracy: {best_accuracy:.2f}% at epoch {best_epoch+1}')
    
    # Load the best model
    model.load_state_dict(best_model)
    
    # Plot training and validation results
    plot_training_results(train_loss, validation_acc, best_epoch, best_accuracy, no_improvement)
    
    return model, train_loss, validation_acc, best_epoch, best_accuracy

def plot_training_results(train_loss, validation_acc, best_epoch, best_acc, no_improvement):
    plt.figure(figsize=(10, 6))
    plt.title('Validation accuracy. Dot denotes best accuracy.')
    plt.plot(validation_acc, label='Validation accuracy')
    plt.plot(best_epoch, best_acc, 'bo', label='Best accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy (%)')
    plt.legend()
    plt.grid(True)
    plt.savefig('validation_accuracy.png')
    plt.show()
    
    plt.figure(figsize=(10, 6))
    plt.title('Training loss')
    plt.plot(train_loss)
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.grid(True)
    plt.savefig('training_loss.png')
    plt.show()
    
    # Plot the last k epochs
    k = max(3 * no_improvement, 0)
    if len(validation_acc) > k:
        plt.figure(figsize=(10, 6))
        plt.title(f'Last {k} epochs')
        plt.plot(validation_acc[-k:])
        plt.plot(best_epoch - (len(validation_acc) - k), best_acc, 'bo')
        plt.xlabel('Epoch')
        plt.ylabel('Accuracy (%)')
        plt.grid(True)
        plt.savefig('last_k_epochs.png')
        plt.show()

def evaluate_model(model, test_loader):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model.to(device)
    model.eval()
    
    correct = 0
    total = 0
    class_correct = [0] * 10
    class_total = [0] * 10
    
    with torch.no_grad():
        for inputs, labels in test_loader:
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = model(inputs)
            _, predicted = torch.max(outputs.data, 1)
            
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
            
            # Calculate per-class accuracy
            for i in range(labels.size(0)):
                label = labels[i]
                class_correct[label] += (predicted[i] == label).item()
                class_total[label] += 1
    
    print(f'Test Accuracy: {100 * correct / total:.2f}%')
    
    for i in range(10):
        if class_total[i] > 0:
            print(f'Accuracy of {class_labels[i]}: {100 * class_correct[i] / class_total[i]:.2f}%')
            
    return 100 * correct / total

def predict_image(model, image_path):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model.to(device)
    model.eval()
    
    try:
        # Load a grayscale image
        try:
            # Try using torchvision's read_image first
            img = io.read_image(image_path, mode=ImageReadMode.GRAY)
            img = img.float() / 255.0  # Normalize to [0, 1]
        except:
            # Fallback to PIL if torchvision fails
            img = Image.open(image_path).convert('L')
            transform = transforms.Compose([
                transforms.Resize((28, 28)),
                transforms.ToTensor()
            ])
            img = transform(img)
        
        # Apply normalization
        img = (img - 0.5) / 0.5
        
        # Add batch dimension if it doesn't have one
        if img.dim() == 2:
            img = img.unsqueeze(0).unsqueeze(0)  # Add batch and channel dimensions
        elif img.dim() == 3:
            img = img.unsqueeze(0)  # Add batch dimension only
            
        img = img.to(device)
        
        # Make prediction
        with torch.no_grad():
            output = model(img)
            _, predicted = torch.max(output, 1)
            label_idx = predicted.item()
            
        return class_labels[label_idx]
    except Exception as e:
        print(f"Error processing image: {e}")
        return None

def main():
    # Set random seed for reproducibility
    torch.manual_seed(42)
    
    # Data transforms
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5,), (0.5,))
    ])
    
    # Load datasets
    DATA_DIR = "."
    print("Loading datasets...")
    
    try:
        train_dataset = datasets.FashionMNIST(DATA_DIR, train=True, download=False, transform=transform)
        test_dataset = datasets.FashionMNIST(DATA_DIR, train=False, download=False, transform=transform)
        
        # Split training data into train and validation - Taken from Tutorial 3 but without hardcoded values
        train_size = int(0.8 * len(train_dataset))
        val_size = len(train_dataset) - train_size
        train_dataset, val_dataset = torch.utils.data.random_split(train_dataset, [train_size, val_size])
        
        # Create data loaders
        train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
        val_loader = DataLoader(val_dataset, batch_size=64, shuffle=False)
        test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)
        
        print(f"Training data: {len(train_dataset)} images")
        print(f"Validation data: {len(val_dataset)} images")
        print(f"Test data: {len(test_dataset)} images")
        
        # Initialize the CNN model
        model = FashionCNN()
        
        # Define loss function and optimizer
        cost = nn.CrossEntropyLoss()
        learning_rate = 0.001
        optimizer = optim.Adam(model.parameters(), lr=learning_rate)
        num_epochs = 20
        no_improvement = 5
        
        # Train the model
        print("Training model...")
        model, train_loss, validation_acc, best_epoch, best_acc = train_model(
            model, train_loader, val_loader, cost, optimizer, num_epochs, no_improvement
        )
        
        # Evaluate on test set
        print("\nEvaluating on test set...")
        test_accuracy = evaluate_model(model, test_loader)
        
        print("\nDone!")
        
        # Interactive prediction loop
        while True:
            image_path = input("Please enter a filepath (or 'exit' to quit): ")
            if image_path.lower() == 'exit':
                print("Exiting...")
                break
            
            prediction = predict_image(model, image_path)
            if prediction:
                print(f"Classifier: {prediction}")
    
    except FileNotFoundError:
        print("Error: FashionMNIST dataset not found. Make sure the dataset is in the current directory.")
        sys.exit(1)
    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()